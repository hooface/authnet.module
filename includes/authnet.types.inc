<?php

/**
 * @file
 * Authorize.net API type wrapper functions
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
function authnet_customer_profile_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetCustomer object.
  $customerProfile = new AuthorizeNetCustomer;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['description'])) {
    $customerProfile->description = $values['description'];
  }
  if (isset($values['merchantCustomerId'])) {
    $customerProfile->merchantCustomerId = $values['merchantCustomerId'];
  }
  if (isset($values['email'])) {
    $customerProfile->email = $values['email'];
  }

  // Return the profile object.
  return $customerProfile;
}