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
  if (isset($values['customerProfileId'])) {
    $customerProfile->customerProfileId = $values['customerProfileId'];
  }
  if (isset($values['paymentProfiles']) && !empty($values['paymentProfiles'])) {
    foreach ($values['paymentProfiles'] as $paymentProfile) {
      $customerProfile->paymentProfiles[] = $paymentProfile;
    }
  }
  if (isset($values['shipToList']) && !empty($values['shipToList'])) {
    foreach ($values['shipToList'] as $shipToList) {
      $customerProfile->shipToList[] = $shipToList;
    }
  }

  // Return the profile object.
  return $customerProfile;
}

/**
 * Returns a new initialized AuthorizeNetAddress object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetAddress object.
 */
function authnet_address_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetAddress object.
  $address = new AuthorizeNetAddress;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['firstName'])) {
    $address->firstName = $values['firstName'];
  }
  if (isset($values['lastName'])) {
    $address->lastName = $values['lastName'];
  }
  if (isset($values['company'])) {
    $address->company = $values['company'];
  }
  if (isset($values['address'])) {
    $address->address = $values['address'];
  }
  if (isset($values['city'])) {
    $address->city = $values['city'];
  }
  if (isset($values['state'])) {
    $address->state = $values['state'];
  }
  if (isset($values['zip'])) {
    $address->zip = $values['zip'];
  }
  if (isset($values['country'])) {
    $address->country = $values['country'];
  }
  if (isset($values['phoneNumber'])) {
    $address->phoneNumber = $values['phoneNumber'];
  }
  if (isset($values['faxNumber'])) {
    $address->faxNumber = $values['faxNumber'];
  }
  if (isset($values['customerAddressId'])) {
    $address->customerAddressId = $values['customerAddressId'];
  }

  // Return the address object.
  return $address;
}

/**
 * Returns a new initialized AuthorizeNetPaymentProfile object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetPaymentProfile object.
 */
function authnet_payment_profile_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetPaymentProfile object.
  $paymentProfile = new AuthorizeNetPaymentProfile;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['customerPaymentProfileId'])) {
    $paymentProfile->customerPaymentProfileId = $values['customerPaymentProfileId'];
  }
  if (isset($values['customerType'])) {
    $paymentProfile->customerType = $values['customerType'];
  }
  if (isset($values['billTo'])) {
    $paymentProfile->billTo = $values['billTo'];
  }
  if (isset($values['payment'])) {
    $paymentProfile->payment = $values['payment'];
  }

  // Return the profile object.
  return $paymentProfile;
}

/**
 * Returns a new initialized AuthorizeNetPayment object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetPayment object.
 */
function authnet_payment_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetPayment object.
  $payment = new AuthorizeNetPayment;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['creditCard'])) {
    $payment->creditCard = $values['creditCard'];
  }
  if (isset($values['bankAccount'])) {
    $payment->bankAccount = $values['bankAccount'];
  }

  // Return the payment object.
  return $payment;
}