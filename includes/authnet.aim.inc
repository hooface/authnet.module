<?php

/**
 * @file
 * Authorize.net API AIM wrapper functions
 */

/**
 * @todo
 * The following AIM features still needs to be implemented:
 *   -Adding line items: AuthorizeNetAIM::addLineItem()
 *   -Setting custom fields: AuthorizeNetAIM::setCustomField()
 *   -eChecks: AuthorizeNetAIM::setECheck()
 *   -Partial authorization transactions.
 */

/**
 * Send a new transaction to Authorize.net.
 *
 * @param $sale
 *   An array of fields to add to the transaction.
 * @param $method
 *   The method of transaction to perform. Must be one of the defined AUTHNET_METHOD_* constants (see authnet.module).
 *
 * @return
 *   Returns the AuthorizeNetAIM_Response object.
 */
function authnet_aim_transaction($sale, $method = AUTHNET_METHOD_AUTH_CAPTURE) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create an Authorize.net AIM request object.
  $request = new AuthorizeNetAIM;

  // Define whether or not to use the Authorize.net sandbox, based on the system setting.
  define("AUTHORIZENET_SANDBOX", variable_get('authnet_aim_sandbox', 0) ? TRUE : FALSE);

  // Set up the fields on the AIM request object.
  $request->setFields($sale);

  // Perform the request based on the method.
  switch ($method) {

    case AUTHNET_METHOD_AUTH_CAPTURE:
      $function = 'authorizeAndCapture';
      break;
    case AUTHNET_METHOD_PRIOR_AUTH_CAPTURE:
      $function = 'priorAuthCapture';
      break;
    case AUTHNET_METHOD_AUTH_ONLY:
      $function = 'authorizeOnly';
      break;
    case AUTHNET_METHOD_CAPTURE_ONLY:
      $function = 'captureOnly';
      break;
    case AUTHNET_METHOD_CREDIT:
      $function = 'credit';
      break;
    case AUTHNET_METHOD_VOID:
      $function = 'void';
      break;
  }

  // If a function is set, send the request to Authorize.net
  if (isset($function)) {
    $response = $request->$function();
  }

  // If the request was unsuccessful, send it to _authnet_error() for reporting.
  if (!$request->approved) {
    _authnet_error($response);
  }

  // Return the response object.
  return $response;
}