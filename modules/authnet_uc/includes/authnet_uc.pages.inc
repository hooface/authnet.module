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

  // Link to general settings.
  $form['authnet'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Authorize.net settings'),
    '#description' => t('General Authorize.net settings can be configured in the ' . l('Authorize.net API settings', 'admin/settings/authnet') . '.'),
  );

  // Ubercart-specific AIM settings
  $form['uc_aim'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ubercart-specific AIM settings'),
    '#description' => t('The following settings apply to Authorize.net AIM transactions conducted by Ubercart.'),
  );

  // AIM email receipt
  $form['uc_aim']['authnet_uc_aim_email_receipt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send the customer an email receipt via Authorize.net when the Advanced Integration Method (AIM) is used.'),
    '#default_value' => variable_get('authnet_uc_aim_email_customer', FALSE),
  );

  // Allow admin to set duplicate window
  $form['uc_aim']['authnet_uc_aim_duplicate_window'] = array(
    '#type' => 'select',
    '#title' => t('Duplicate window'),
    '#description' => t('Blocks submission of duplicate transactions within the specified window.  Defaults to 120 seconds.'),
    '#default_value' => variable_get('authnet_uc_aim_duplicate_window', 120),
    '#options' => drupal_map_assoc(array(0, 15, 30, 45, 60, 75, 90, 105, 120)),
  );

  return $form;
}
