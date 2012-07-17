<?php

/**
 * @file
 * Authorize.net - User Pages
 */

/**
 * User Billing page
 */
function authnet_user_billing_page($user){
  // Load the customer profile from Authorize.net.
  $customer_profile = authnet_cim_entity_profile_load('user', $user->uid);

  if (empty($customer_profile) || empty($customer_profile->paymentProfiles))
    return t('No payment profiles found');

  $title = t('Existing payment profiles');
  $profiles_list = authnet_ui_payment_profile_options($customer_profile);

  return theme('item_list', $profiles_list, $title);
}
