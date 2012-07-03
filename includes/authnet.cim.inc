<?php

/**
 * @file
 * Authorize.net API CIM wrapper functions
 */

/* ******************************************************************************
 * CIM customer profile wrapper functions
 * *****************************************************************************/

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

    // Define the non-unique indices to look for (paymentProfiles and shipToList)
    $nonqunique = array('paymentProfiles', 'shipToList');

    // Convert the profile from a SimpleXMLElement to an AuthorizeNetCustomer object.
    $customerProfile = authnet_customer_profile_new(_authnet_simplexml_to_array($response->xml->profile, $nonqunique));

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

      // Invoke hook_authnet_cim_profile_insert and pass in the customer profile object.
      module_invoke_all('authnet_cim_profile_insert', $customerProfile);

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

/* ******************************************************************************
 * CIM payment profile wrapper functions
 * *****************************************************************************/

/**
 * Load an existing customer payment profile from Authorize.net.
 *
 * @param $customerProfileId
 *   The customer profile id.
 * @param $customerPaymentProfileId
 *   The customer payment profile id.
 *
 * @return
 *   Returns an AuthorizeNetPaymentProfile object if available, NULL otherwise.
 */
function authnet_cim_payment_profile_load($customerProfileId, $customerPaymentProfileId) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net request object.
  $request = new AuthorizeNetCIM;

  // Retrieve the customer profile.
  $response = $request->getCustomerPaymentProfile($customerProfileId, $customerPaymentProfileId);

  // If the request was successful, return the profile.
  if ($response->isOK()) {

    // Convert the payment profile from a SimpleXMLElement to an AuthorizeNetPaymentProfile object.
    $paymentProfile = authnet_payment_profile_new(_authnet_simplexml_to_array($response->xml->paymentProfile));

    // Return the profile.
    return $paymentProfile;
  }

  // If the request was unsuccessful, send it to _authnet_error() for reporting.
  else {
    _authnet_error($response);
  }
}

/**
 * Save a payment profile to Authorize.net.
 * If the $paymentProfile->customerPaymentProfileId propery is set, then the profile will be updated.
 * Otherwise, a new one will be saved.
 *
 * @param $customerProfileId
 *   The customer profile id.
 * @param AuthorizeNetPaymentProfile &$paymentProfile
 *   The payment profile object to be saved, passed by reference.
 *
 * @return
 *   Returns an AuthorizeNetPaymentProfile object if available, NULL otherwise.
 */
function authnet_cim_payment_profile_save($customerProfileId, AuthorizeNetPaymentProfile &$paymentProfile) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net request object.
  $request = new AuthorizeNetCIM;

  // If the $paymentProfile->customerPaymentProfileId propery is set, then the profile will be updated.
  if (isset($paymentProfile->customerPaymentProfileId) && !empty($paymentProfile->customerPaymentProfileId)) {

    // Update the profile.
    $response = $request->updateCustomerPaymentProfile($customerProfileId, $paymentProfile->customerPaymentProfileId, $paymentProfile);
  }

  // Otherwise, save a new payment profile.
  else {

    // Save the profile.
    $response = $request->createCustomerPaymentProfile($customerProfileId, $paymentProfile);

    // If the request was successful...
    if ($response->isOk()) {

      // Get the new profile id from the response.
      $customerPaymentProfileId = $response->getPaymentProfileId();

      // Load the full payment profile object with another call to Authorize.net.
      $paymentProfile = authnet_cim_payment_profile_load($customerProfileId, $customerPaymentProfileId);

      // Return the profile.
      return $paymentProfile;
    }
  }

  // If the request was unsuccessful, send it to _authnet_error() for reporting.
  if ($response->isError()) {
    _authnet_error($response);
  }
}

/**
 * Delete a payment profile from Authorize.net
 *
 * @param $customerProfileId
 *   The customer profile id.
 * @param $customerPaymentProfileId
 *   The id of the profile profile to be deleted.
 */
function authnet_cim_payment_profile_delete($customerProfileId, $customerPaymentProfileId) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net request object.
  $request = new AuthorizeNetCIM;

  // Delete the profile.
  $response = $request->deleteCustomerPaymentProfile($customerProfileId, $customerPaymentProfileId);

  // If the request was unsuccessful, send it to _authnet_error() for reporting.
  if (!$response->isOk()) {
    _authnet_error($response);
  }
}

/* ******************************************************************************
 * CIM shipping address wrapper functions
 * *****************************************************************************/

/**
 * Load an existing customer shipping address from Authorize.net.
 *
 * @param $customerProfileId
 *   The customer profile id.
 * @param $customerAddressId
 *   The customer address id id.
 *
 * @return
 *   Returns an AuthorizeNetAddress object if available, NULL otherwise.
 */
function authnet_cim_shipping_address_load($customerProfileId, $customerAddressId) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net request object.
  $request = new AuthorizeNetCIM;

  // Retrieve the customer shipping address.
  $response = $request->getCustomerShippingAddress($customerProfileId, $customerAddressId);

  // If the request was successful, return the address.
  if ($response->isOK()) {

    // Convert the address from a SimpleXMLElement to an AuthorizeNetAddress object.
    $address = authnet_address_new(_authnet_simplexml_to_array($response->xml->address));

    // Return the address.
    return $address;
  }

  // If the request was unsuccessful, send it to _authnet_error() for reporting.
  else {
    _authnet_error($response);
  }
}

/**
 * Save a shipping address to Authorize.net.
 * If the $address->customerAddressId propery is set, then the address will be updated.
 * Otherwise, a new one will be saved.
 *
 * @param $customerProfileId
 *   The customer profile id.
 * @param AuthorizeNetAddress &$address
 *   The address object to be saved, passed by reference.
 *
 * @return
 *   Returns an AuthorizeNetAddress object if available, NULL otherwise.
 */
function authnet_cim_shipping_address_save($customerProfileId, AuthorizeNetAddress &$address) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net request object.
  $request = new AuthorizeNetCIM;

  // If the $address->customerAddressId propery is set, then the profile will be updated.
  if (isset($address->customerAddressId) && !empty($address->customerAddressId)) {

    // Update the profile.
    $response = $request->updateCustomerShippingAddress($customerProfileId, $address->customerAddressId, $address);
  }

  // Otherwise, save a new address.
  else {

    // Save the address.
    $response = $request->createCustomerShippingAddress($customerProfileId, $address);

    // If the request was successful...
    if ($response->isOk()) {

      // Get the new address id from the response.
      $customerAddressId = $response->getCustomerAddressId();

      // Load the full address object with another call to Authorize.net.
      $address = authnet_cim_shipping_address_load($customerProfileId, $customerAddressId);

      // Return the address.
      return $address;
    }
  }

  // If the request was unsuccessful, send it to _authnet_error() for reporting.
  if ($response->isError()) {
    _authnet_error($response);
  }
}

/**
 * Delete a shipping address from Authorize.net
 *
 * @param $customerProfileId
 *   The customer profile id.
 * @param $customerAddressId
 *   The customer address id.
 */
function authnet_cim_shipping_address_delete($customerProfileId, $customerAddressId) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net request object.
  $request = new AuthorizeNetCIM;

  // Delete the address.
  $response = $request->deleteCustomerShippingAddress($customerProfileId, $customerAddressId);

  // If the request was unsuccessful, send it to _authnet_error() for reporting.
  if (!$response->isOk()) {
    _authnet_error($response);
  }
}