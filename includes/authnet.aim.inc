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
 *   Returns the AuthorizeNetAIM_Response object, NULL if an invalid method is specified.
 */
function authnet_aim_transaction($sale, $method = AUTHNET_METHOD_AUTH_CAPTURE) {

  // Define the available transaction methods.
  $methods = array(
    AUTHNET_METHOD_AUTH_CAPTURE => 'authorizeAndCapture',
    AUTHNET_METHOD_PRIOR_AUTH_CAPTURE => 'priorAuthCapture',
    AUTHNET_METHOD_AUTH_ONLY => 'authorizeOnly',
    AUTHNET_METHOD_CAPTURE_ONLY => 'captureOnly',
    AUTHNET_METHOD_CREDIT => 'credit',
    AUTHNET_METHOD_VOID => 'void',
  );

  // If a valid method is set...
  if (isset($methods[$method])) {

    // Create an Authorize.net request object.
    $request = authnet_request_new('AIM');

    // Set up the fields on the AIM request object.
    $request->setFields($sale);

    // Send the request to Authorize.net
    $response = $request->$methods[$method]();

    // Log the response object.
    authnet_log_response($response);

    // If the request was unsuccessful, send it to _authnet_error() for reporting.
    if (!$request->approved) {
      _authnet_error($response);
    }

    // Return the response object.
    return $response;
  }
}