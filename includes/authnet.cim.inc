<?php

/**
 * @file
 * Authorize.net API wrapper functions
 */

/**
 * Returns a new initialized AuthorizeNetCustomer object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetCustomer object.
 */
function authnet_cim_profile_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetCustomer object.
  $customerProfile = new AuthorizeNetCustomer;

  // Initialize some properties, using the $values argument if it's available.
  $customerProfile->description = isset($values['description']) ? $values['description'] : '';
  $customerProfile->merchantCustomerId = isset($values['merchantCustomerId']) ? $values['merchantCustomerId'] : '';
  $customerProfile->email = isset($values['email']) ? $values['email'] : '';

  // Return the profile object.
  return $customerProfile;
}

/**
 * Load an existing customer profile from Authorize.net.
 *
 * @param $customerProfileId
 *   The customer profile id.
 *
 * @return
 *   Returns an AuthorizeNetCustomer object if available, NULL otherwise.
 */
function authnet_cim_profile_load($customerProfileId) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net request object.
  $request = new AuthorizeNetCIM;

  // Retrieve the customer profile.
  $response = $request->getCustomerProfile($customerProfileId);

  // If the request was successful, return the profile.
  if ($response->isOK()) {

    // Convert the profile from a SimpleXMLElement to an AuthorizeNetCustomer object.
    $customerProfile = authnet_cim_profile_new();
    _authnet_convert_simplexml($response->xml->profile, $customerProfile);

    // Invoke hook_authnet_cim_profile_load and pass in the customer profile object.
    module_invoke_all('authnet_cim_profile_load', $customerProfile);

    // Return the profile.
    return $customerProfile;
  }

  // If the request was unsuccessful, send it to _authnet_error() for reporting.
  else {
    _authnet_error($response);
  }
}

/**
 * Save a customer profile to Authorize.net.
 * If the $customerProfile->customerProfileId propery is set, then the profile will be updated.
 * Otherwise, a new one will be saved.
 *
 * @param AuthorizeNetCustomer &$customerProfile
 *   The customer profile object to be saved, passed by reference.
 *
 * @return
 *   Returns an AuthorizeNetCustomer object if available, NULL otherwise.
 */
function authnet_cim_profile_save(AuthorizeNetCustomer &$customerProfile) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net request object.
  $request = new AuthorizeNetCIM;

  // If the $customerProfile->customerProfileId propery is set, then the profile will be updated.
  if (isset($customerProfile->customerProfileId) && !empty($customerProfile->customerProfileId)) {

    // Update the profile.
    $response = $request->updateCustomerProfile($customerProfile->customerProfileId, $customerProfile);
  }

  // Otherwise, save a new customer profile.
  else {

    // Save the profile.
    $response = $request->createCustomerProfile($customerProfile);

    // If the request was successful...
    if ($response->isOk()) {

      // Get the new profile id from the response.
      $customerProfileId = $response->getCustomerProfileId();

      // Load the full customer profile object with another call to Authorize.net.
      $customerProfile = authnet_cim_profile_load($customerProfileId);

      // Invoke hook_authnet_cim_profile_save and pass in the customer profile object.
      module_invoke_all('authnet_cim_profile_save', $customerProfile);

      // Return the profile.
      return $customerProfile;
    }
  }

  // If the request was unsuccessful, send it to _authnet_error() for reporting.
  if ($response->isError()) {
    _authnet_error($response);
  }
}

/**
 * Delete a customer profile from Authorize.net
 *
 * @param $customerProfileId
 *   The id of the customer profile to be deleted.
 */
function authnet_cim_profile_delete($customerProfileId) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net request object.
  $request = new AuthorizeNetCIM;

  // Delete the profile.
  $response = $request->deleteCustomerProfile($customerProfileId);

  // If the request was successful, invoke hook_authnet_cim_profile_delete and pass in the customer profile id.
  if ($response->isOk()) {
    module_invoke_all('authnet_cim_profile_delete', $customerProfileId);
  }

  // If the request was unsuccessful, send it to _authnet_error() for reporting.
  else {
    _authnet_error($response);
  }
}