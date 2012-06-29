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