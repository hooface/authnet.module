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
    authnet_convert_simplexml($response->xml->profile, $customerProfile);

    // Return the profile.
    return $customerProfile;
  }

  // If the request was unsuccessful, send it to authnet_error() for reporting.
  else {
    authnet_error($response);
  }
}

/**
 * Save a customer profile to Authorize.net.
 *
 * @param AuthorizeNetCustomer $customerProfile
 *   The customer profile object to save be saved.
 *
 * @return
 *   Returns an AuthorizeNetCIM_Response object.
 */
function authnet_cim_profile_save(AuthorizeNetCustomer $customerProfile) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net request object.
  $request = new AuthorizeNetCIM;

  // Create the new customer profile.
  $response = $request->createCustomerProfile($customerProfile);

  // Return the response.
  return $response;
}