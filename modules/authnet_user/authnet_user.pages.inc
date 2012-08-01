<?php

/**
 * @file
 * Authorize.net - User Pages
 */

/**
 * User Billing page
 */
function authnet_user_billing_page($user) {

  drupal_set_title('Saved cards');

  // Start output.
  $output = '';

  // Create a link to add a new profile.
  $output .= l('+ Add a credit card', 'user/' . $user->uid . '/billing/add');

  // Load the customer profile from Authorize.net.
  $customer_profile = authnet_cim_entity_profile_load('user', $user->uid);

  // Display an empty text message if no profiles are available.
  if (empty($customer_profile) || empty($customer_profile->paymentProfiles)) {
    $output .= t('You don\'t have any saved cards. Click "Add a credit card" above to save a new one.');
  }

  // Load the list of payment profiles.
  $profiles_list = authnet_ui_payment_profile_options($customer_profile);

  // Put together header and rows for the payment profile table.
  $header = array(
    t('Credit card'),
    t('Operations'),
  );
  $rows = array();
  foreach ($profiles_list as $payment_profile) {
    $rows[] = array(
      l($payment_profile, 'user/' . $user->uid . '/billing/edit'),
      l('Edit', 'user/' . $user->uid . '/billing/edit') . ' | ' . l('Delete', 'user/' . $user->uid . '/billing/delete'),
    );
  }

  // Add the themed table to the output.
  $output .= theme('table', $header, $rows);

  // Return the output.
  return $output;
}

/**
 * User payment profile form.
 *
 * @param $user
 *   The user account object.
 * @param $pid
 *   The id of the payment profile database record (optional).
 */
function authnet_user_payment_profile_form(&$form_state, $user, $pid = NULL) {

  // Set the page title.
  if ($pid) {
    drupal_set_title(t('Edit payment profile'));
  }
  else {
    drupal_set_title(t('Add a payment profile'));
  }

  // Load the user's customer profile id.
  $customer_profile_id = authnet_cim_entity_profile_id_load('user', $user->uid);

  // Load the payment profile id.
  $payment_profile_id = NULL;
  if ($pid) {
    $payment_profile_id = authnet_cim_entity_payment_profile_id_load($pid);
  }

  // Start with an empty form.
  $form = array();

  // Save the user id, customer profile id, and payment profile id to the form state.
  $form_state['storage']['uid'] = $user->uid;
  $form_state['storage']['customer_profile_id'] = $customer_profile_id;
  $form_state['storage']['payment_profile_id'] = $payment_profile_id;

  // Add the Authnet UI payment profile form.
  $form += authnet_ui_payment_profile_form($customer_profile_id, $payment_profile_id);

  // Return the payment profile form.
  return $form;
}

/**
 * Payment profile form submit.
 * This is added to the payment profile form via hook_form_alter().
 * @see authnet_user_form_alter()
 */
function authnet_user_payment_profile_form_submit($form, &$form_state) {

  // Load the user object.
  $user = user_load($form_state['storage']['uid']);

  // If a customer profile id is not available, create a new profile.
  $customer_profile_id = $form_state['storage']['customer_profile_id'];
  if (!$customer_profile_id) {
    $customer_profile = authnet_user_customer_profile_new($user);
    $customer_profile_id = $customer_profile->customer_profile_id;
  }

  // Assemble the payment profile object from the form values.
  $values = array(
    'customerPaymentProfileId' => $form_state['storage']['payment_profile_id'],
    'customerType' => check_plain($form_state['values']['customer_type']),
    'billTo' => array(
      'firstName' => check_plain($form_state['values']['billing']['first_name']),
      'lastName' => check_plain($form_state['values']['billing']['last_name']),
      'company' => check_plain($form_state['values']['billing']['company']),
      'address' => check_plain($form_state['values']['billing']['address']),
      'city' => check_plain($form_state['values']['billing']['city']),
      'state' => check_plain($form_state['values']['billing']['state']),
      'zip' => check_plain($form_state['values']['billing']['zip']),
      'country' => check_plain($form_state['values']['billing']['country']),
      'phoneNumber' => check_plain($form_state['values']['billing']['phone']),
      'faxNumber' => check_plain($form_state['values']['billing']['fax']),
    ),
    'payment' => array(
      'creditCard' => array(
        'cardNumber' => check_plain($form_state['values']['card']['number']),
        'expirationDate' => check_plain($form_state['values']['card']['exp_year']) . '-' . check_plain($form_state['values']['card']['exp_month']),
        'cardCode' => check_plain($form_state['values']['card']['cvv']),
      ),
    ),
  );
  $payment_profile = authnet_payment_profile_new($values);

  // Save the payment profile.
  authnet_cim_payment_profile_save($customer_profile_id, $payment_profile);

  /**
   * Authorize.net does not return the expiration date when it saves a payment profile, nor does it save
   * the credit card type, so those pieces of information are missing from the database record that the authnet_cim_entity
   * module creates. Therefore, we need to add it here.
   * It's less than ideal that we need to save the payment profile record twice like this, but it's the only way to ensure
   * that the values are saved to the database, while still maintaining the ability to use the authnet_cim_entity module
   * without the authnet_user module.
   */

  // Load the record that was saved to the database.
  $record = authnet_cim_entity_payment_profile_load($payment_profile->customerPaymentProfileId);
  if ($record) {

    // Set a flag for whether or not we need to save the record.
    $save = FALSE;

    // Set the card type, if it's different from the saved record, and not empty.
    if (!empty($form_state['values']['card']['type']) && $record['cc_type'] != $form_state['values']['card']['type']) {
      $record['cc_type'] = check_plain($form_state['values']['card']['type']);
      $save = TRUE;
    }

    // Set the expiration month and year, if they are different from the saved record, and are not empty.
    if ( (!empty($form_state['values']['card']['exp_month']) && !empty($form_state['values']['card']['exp_year'])) && ( ($record['cc_exp_month'] != $form_state['values']['card']['exp_month']) || ($record['cc_exp_year'] != $form_state['values']['card']['exp_year']) ) ) {
      $record['cc_exp_month'] = check_plain($form_state['values']['card']['exp_month']);
      $record['cc_exp_year'] = check_plain($form_state['values']['card']['exp_year']);
      $save = TRUE;
    }

    // Save the record.
    if ($save) {
      authnet_cim_entity_payment_profile_save($record);
    }
  }
}