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
 *   Returns an AuthorizeNetCustomer object if available, FALSE otherwise.
 */
function authnet_cim_profile_load($customerProfileId) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Retrieve the customer profile.
  $response = $request->getCustomerProfile($customerProfileId);

  // Log the response object.
  authnet_log_response($response);

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

  // Return FALSE if the above didn't return anything else.
  return FALSE;
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
 *   Returns TRUE if the save was successful, FALSE otherwise.
 */
function authnet_cim_profile_save(AuthorizeNetCustomer &$customerProfile) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Determine the validation mode.
  $validation = variable_get('authnet_cim_validation_mode', 'none');
  /**
   * @todo
   * The validation mode is currently passed in, but the module doesn't handle the validation responses yet.
   * Use $response->getValidationResponses() to retrieve validation responses, and then parse/display them.
   */

  // If the $customerProfile->customerProfileId propery is set, then update an existing profile.
  if (isset($customerProfile->customerProfileId) && !empty($customerProfile->customerProfileId)) {
    $response = $request->updateCustomerProfile($customerProfile->customerProfileId, $customerProfile, $validation);
    $new = FALSE;
  }

  // Otherwise, save a new customer profile.
  else {
    $response = $request->createCustomerProfile($customerProfile, $validation);
    $new = TRUE;
  }

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful...
  if ($response->isOk()) {

    // If a new profile was saved...
    if ($new) {

      // Get the new profile id from the response.
      $customerProfileId = $response->getCustomerProfileId();

      // Load the full customer profile object with another call to Authorize.net.
      $customerProfile = authnet_cim_profile_load($customerProfileId);

      // Invoke hook_authnet_cim_profile_insert and pass in the customer profile object.
      module_invoke_all('authnet_cim_profile_insert', $customerProfile);
    }

    // If the profile is being updated...
    else {

      // Invoke hook_authnet_cim_profile_update and pass in the customer profile object.
      module_invoke_all('authnet_cim_profile_update', $customerProfile);
    }

    // Return TRUE.
    return TRUE;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/**
 * Delete a customer profile from Authorize.net
 *
 * @param $customerProfileId
 *   The id of the customer profile to be deleted.
 *
 * @return
 *   Returns TRUE on success, FALSE otherwise.
 */
function authnet_cim_profile_delete($customerProfileId) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Delete the profile.
  $response = $request->deleteCustomerProfile($customerProfileId);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful...
  if ($response->isOk()) {

    // Invoke hook_authnet_cim_profile_delete and pass in the customer profile id.
    module_invoke_all('authnet_cim_profile_delete', $customerProfileId);

    // Return TRUE.
    return TRUE;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
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
 *   Returns an AuthorizeNetPaymentProfile object if available, FALSE otherwise.
 */
function authnet_cim_payment_profile_load($customerProfileId, $customerPaymentProfileId) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Retrieve the customer profile.
  $response = $request->getCustomerPaymentProfile($customerProfileId, $customerPaymentProfileId);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful, return the profile.
  if ($response->isOK()) {

    // Convert the payment profile from a SimpleXMLElement to an AuthorizeNetPaymentProfile object.
    $paymentProfile = authnet_payment_profile_new(_authnet_simplexml_to_array($response->xml->paymentProfile));

    // Return the profile.
    return $paymentProfile;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
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
 *   Returns an AuthorizeNetPaymentProfile object if available, FALSE otherwise.
 */
function authnet_cim_payment_profile_save($customerProfileId, AuthorizeNetPaymentProfile &$paymentProfile) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Determine the validation mode.
  $validation = variable_get('authnet_cim_validation_mode', 'none');
  /**
   * @todo
   * The validation mode is currently passed in, but the module doesn't handle the validation responses yet.
   * Use $response->getValidationResponses() to retrieve validation responses, and then parse/display them.
   */

  // If the $paymentProfile->customerPaymentProfileId propery is set, then the profile will be updated.
  if (isset($paymentProfile->customerPaymentProfileId) && !empty($paymentProfile->customerPaymentProfileId)) {
    $response = $request->updateCustomerPaymentProfile($customerProfileId, $paymentProfile->customerPaymentProfileId, $paymentProfile, $validation);
    $new = FALSE;
  }

  // Otherwise, save a new payment profile.
  else {
    $response = $request->createCustomerPaymentProfile($customerProfileId, $paymentProfile, $validation);
    $new = TRUE;
  }

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful...
  if ($response->isOk()) {

    // If a new profile was created...
    if ($new) {

      // Get the new profile id from the response.
      $customerPaymentProfileId = $response->getPaymentProfileId();

      // Load the full payment profile object with another call to Authorize.net.
      $paymentProfile = authnet_cim_payment_profile_load($customerProfileId, $customerPaymentProfileId);
    }

    // Return TRUE.
    return TRUE;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/**
 * Delete a payment profile from Authorize.net
 *
 * @param $customerProfileId
 *   The customer profile id.
 * @param $customerPaymentProfileId
 *   The id of the profile profile to be deleted.
 *
 * @return
 *   Returns TRUE if successful, FALSE otherwise.
 */
function authnet_cim_payment_profile_delete($customerProfileId, $customerPaymentProfileId) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Delete the profile.
  $response = $request->deleteCustomerPaymentProfile($customerProfileId, $customerPaymentProfileId);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful, return TRUE.
  if ($response->isOk()) {
    return TRUE;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
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
 *   Returns an AuthorizeNetAddress object if available, FALSE otherwise.
 */
function authnet_cim_shipping_address_load($customerProfileId, $customerAddressId) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Retrieve the customer shipping address.
  $response = $request->getCustomerShippingAddress($customerProfileId, $customerAddressId);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful, return the address.
  if ($response->isOK()) {

    // Convert the address from a SimpleXMLElement to an AuthorizeNetAddress object.
    $address = authnet_address_new(_authnet_simplexml_to_array($response->xml->address));

    // Return the address.
    return $address;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
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
 *   Returns an AuthorizeNetAddress object if available, FALSE otherwise.
 */
function authnet_cim_shipping_address_save($customerProfileId, AuthorizeNetAddress &$address) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // If the $address->customerAddressId propery is set, then the profile will be updated.
  if (isset($address->customerAddressId) && !empty($address->customerAddressId)) {
    $response = $request->updateCustomerShippingAddress($customerProfileId, $address->customerAddressId, $address);
    $new = FALSE;
  }

  // Otherwise, save a new address.
  else {
    $response = $request->createCustomerShippingAddress($customerProfileId, $address);
    $new = TRUE;
  }

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful...
  if ($response->isOk()) {

    // If a new address was created...
    if ($new) {

      // Get the new address id from the response.
      $customerAddressId = $response->getCustomerAddressId();

      // Load the full address object with another call to Authorize.net.
      $address = authnet_cim_shipping_address_load($customerProfileId, $customerAddressId);
    }

    // Return the address.
    return $address;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/**
 * Delete a shipping address from Authorize.net
 *
 * @param $customerProfileId
 *   The customer profile id.
 * @param $customerAddressId
 *   The customer address id.
 *
 * @return
 *   Returns TRUE if successful, FALSE otherwise.
 */
function authnet_cim_shipping_address_delete($customerProfileId, $customerAddressId) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Delete the address.
  $response = $request->deleteCustomerShippingAddress($customerProfileId, $customerAddressId);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful, return TRUE.
  if ($response->isOk()) {
    return TRUE;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/* ******************************************************************************
 * CIM transaction wrapper functions
 * *****************************************************************************/

/**
 * Send a new transaction to Authorize.net.
 *
 * @param $transaction
 *   The AuthorizeNetTransaction object that contains all of the transaction data.
 * @param $method
 *   The method of transaction to perform. Must be one of the defined AUTHNET_METHOD_* constants (see authnet.module).
 *
 * @return
 *   Returns TRUE if successful, FALSE otherwise.
 */
function authnet_cim_transaction(AuthorizeNetTransaction $transaction, $method = AUTHNET_METHOD_AUTH_CAPTURE) {

  // Define the available transaction methods.
  $methods = array(
    AUTHNET_METHOD_AUTH_CAPTURE => 'AuthCapture',
    AUTHNET_METHOD_PRIOR_AUTH_CAPTURE => 'PriorAuthCapture',
    AUTHNET_METHOD_AUTH_ONLY => 'AuthOnly',
    AUTHNET_METHOD_CAPTURE_ONLY => 'CaptureOnly',
    AUTHNET_METHOD_CREDIT => 'Refund',
    AUTHNET_METHOD_VOID => 'Void',
  );

  // If a valid method is set...
  if (isset($methods[$method])) {

    // Create an Authorize.net request object.
    $request = authnet_request_new('CIM');

    // Send the transaction to Authorize.net.
    $response = $request->createCustomerProfileTransaction($methods[$method], $transaction);

    // Log the response object.
    authnet_log_response($response);

    // If the request was successful, return TRUE.
    if ($response->isOk()) {
      return TRUE;
    }
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}