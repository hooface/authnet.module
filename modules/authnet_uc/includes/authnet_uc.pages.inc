<?php

/**
 * @file
 * Authorize.net - Ubercart Integration - Pages
 *
 * Includes page callbacks for Authorize.Net's Silent POST feature and user
 * specific recurring fee operation pages.
 */


/* ******************************************************************************
 * Forms
 * *****************************************************************************/

/**
 * Helper to authnet_uc_settings_form().
 * (Callback for payment gateway settings)
 *
 * @TODO:
 * Most of this form should probably come directly from the authnet module itself
 * Consider whether it makes sense to allow for seperate config for authnet core and authnet_uc
 * For sites migrating from uc_authorizenet - consider defaulting to the existing config
 */
function _authnet_uc_settings_form() {
  $login_data = _authnet_uc_login_data();

  $form['api_id_key'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Login ID and Transaction Key'),
    '#description' => t('This information is required for Ubercart to interact with your payment gateway account.  It is different from your login ID and password and may be found through your account settings page.'),
  );
  $form['api_id_key']['authnet_uc_api_login_id'] = array(
    '#type' => 'textfield',
    '#title' => t('API Login ID'),
    '#default_value' => variable_get('authnet_uc_api_login_id', ''),
  );
  $form['api_id_key']['authnet_uc_api_transaction_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Transaction Key'),
    '#default_value' => variable_get('authnet_uc_api_transaction_key', ''),
  );

  $form['aim_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('AIM settings'),
    '#description' => t('These settings pertain to the Authorize.Net AIM payment method for card not present transactions.'),
  );
  $form['aim_settings']['authnet_uc_aim_txn_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Transaction mode'),
    '#description' => t('Only specify a developer test account if you login to your account through https://test.authorize.net.<br/>Adjust to live transactions when you are ready to start processing real payments.'),
    '#options' => array(
      'live' => t('Live transactions in a live account'),
      'live_test' => t('Test transactions in a live account'),
      'developer_test' => t('Developer test account transactions'),
    ),
    '#default_value' => variable_get('authnet_uc_aim_txn_mode', 'live_test'),
  );

  $form['aim_settings']['authnet_uc_aim_email_customer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tell Authorize.net to e-mail the customer a receipt based on your account settings.'),
    '#default_value' => variable_get('authnet_uc_aim_email_customer', FALSE),
  );
  $form['aim_settings']['authnet_uc_response_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log full API response messages from Authorize.net for debugging.'),
    '#default_value' => variable_get('authnet_uc_response_debug', FALSE),
  );


  $form['arb_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('ARB settings'),
    '#description' => t('These settings pertain to the Authorize.Net Automated Recurring Billing service.')
  );
  $form['arb_settings']['authnet_uc_arb_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Transaction mode'),
    '#description' => t('Only specify developer mode if you login to your account through https://test.authorize.net.<br />Adjust to production mode when you are ready to start processing real recurring fees.'),
    '#options' => array(
      'production' => t('Production'),
      'developer' => t('Developer test'),
      'disabled' => t('Disabled'),
    ),
    '#default_value' => variable_get('authnet_uc_arb_mode', 'disabled'),
  );
  $form['arb_settings']['authnet_uc_md5_hash'] = array(
    '#type' => 'textfield',
    '#title' => t('MD5 Hash'),
    '#description' => t('<b>Note:</b> You must first configure credit card encryption before setting this.<br />Enter the value here you entered in your Auth.Net account settings.'),
    '#default_value' => $login_data['md5_hash'],
    '#access' => user_access('administer credit cards'),
  );
  $form['arb_settings']['authnet_uc_report_arb_post'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log reported ARB payments in watchdog.'),
    '#description' => t('Make sure you have set your Silent POST URL in Authorize.Net to @url.', array('@url' => url('authnet/silent-post', array('absolute' => TRUE)))),
    '#default_value' => variable_get('authnet_uc_report_arb_post', FALSE),
  );

  $form['cim_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('CIM settings'),
    '#description' => t('These settings pertain to the Authorize.Net Customer Information Management service.')
  );
  $form['cim_settings']['authnet_uc_cim_profile'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always create a CIM profile for securely storing CC info for later use.'),
    '#default_value' => variable_get('authnet_uc_cim_profile', FALSE),
  );
  $form['cim_settings']['authnet_uc_cim_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Transaction mode'),
    '#description' => t('Only specify a developer test account if you login to your account through https://test.authorize.net.<br/>Adjust to live transactions when you are ready to start processing real payments.'),
    '#options' => array(
      'production' => t('Production'),
      'developer' => t('Developer test'),
      'disabled' => t('Disabled'),
    ),
    '#default_value' => variable_get('authnet_uc_cim_mode', 'disabled'),
  );

  return $form;
}

/* ******************************************************************************
 * Helpers
 * *****************************************************************************/

/**
 * Decrypts the login data for using Auth.Net APIs.
 */
function _authnet_uc_login_data() {
  static $data;

  if (!empty($data)) {
    return $data;
  }

  $md5_hash = variable_get('authnet_uc_md5_hash', '');

  // If CC encryption has been configured properly.
  if ($key = uc_credit_encryption_key()) {
    // Setup our encryption object.
    $crypt = new uc_encryption_class;

    // Decrypt the MD5 Hash.
    if (!empty($md5_hash)) {
      $md5_hash = $crypt->decrypt($key, $md5_hash);
    }

    // Store any errors.
    uc_store_encryption_errors($crypt, 'authnet_uc');
  }

  $data = array(
    'md5_hash' => $md5_hash,
  );

  return $data;
}