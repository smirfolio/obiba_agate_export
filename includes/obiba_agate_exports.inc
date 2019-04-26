<?php
/**
 * @file
 * Obiba Agate Module.
 *
 * Copyright (c) 2015 OBiBa. All rights reserved.
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Class ObibaAgateClient.
 */

class ObibaExport extends ObibaAgateClient{

  public function __construct($agate_url = NULL, $application_name = NULL, $application_key = NULL) {
    parent::__construct($agate_url, $application_name, $application_key);
  }

  /*
   * Export from drupal user to Agate
   * */
  public function exportAgateUser(array $user_data){
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/users';
    $data = json_encode($user_data);

    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_POST,
      'headers' => array(
        'Accept' => array(ObibaAgateClient::HEADER_JSON),
        'Content-Type' => array(ObibaAgateClient::HEADER_JSON),
        ObibaAgateClient::AUTHORIZATION_HEADER => array($this->authorizationHeader()),
      ),
      'data' => $data,
    ));
    $client = $this->client();
    return $this->executeRequest($client, $request);
  }

  /*
   * Send a reset password request
   * */
  public function resetPassword($user_name){
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/user/' . $user_name . '/reset_password';
    $data = json_encode(array('id' => $user_name));

    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_PUT,
      'headers' => array(
        'Accept' => array(ObibaAgateClient::HEADER_JSON),
        'Content-Type' => array(ObibaAgateClient::HEADER_JSON),
        ObibaAgateClient::AUTHORIZATION_HEADER => array($this->authorizationHeader()),
      ),
      'data' => $data,
    ));
    $client = $this->client();
    return $this->executeRequest($client, $request);
  }

  /*
   * Perform request
   * */
  protected function executeRequest($client, $request){
    try {
      $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      $this->setLastResponseCookies();
      return json_decode($this->lastResponse);
    }
    catch (HttpClientException $e) {
      $error_message = json_decode($e->getResponse()->body);
      watchdog('Agate Client', 'Request failed, @code: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      drupal_add_http_header('Status', $e->getCode());
      return array(
        'codeError' => $e->getCode(),
        'message' => $error_message->message,
      );
    }
  }

}