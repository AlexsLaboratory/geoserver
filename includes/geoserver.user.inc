<?php

use Lowem\GeoserverPHP\Workspaces;
use Lowem\EasyCurl\HTTPRequestException;

$modulePath = drupal_get_path("module", "geoserver");
require_once "$modulePath/includes/functions.php";
require_once "$modulePath/vendor/autoload.php";

function geoserver_user_list() {
  $output = "<ul>";
  $output .= "<li>" . l("Create Workspace", "geoserver/call/create-workspace") . "</li>";
  $output .= "</ul>";
  return $output;
}

function geoserver_create_workspace_form($form, &$form_state) {
  $form["workspace_name"] = [
    "#type" => "textfield",
    "#title" => "Workspace Name",
  ];

  $form["submit"] = [
    "#type" => "submit",
    "#name" => "submit",
    "#value" => "Submit"
  ];

  return $form;
}

function geoserver_create_workspace_form_submit($form, &$form_state) {
  $form_state["rebuild"] = FALSE;
  $form_state["redirect"] = "geoserver/calls";
  $workspace_name = $form_state["values"]["workspace_name"];
  $userCreds = get_use_cred();

  $workspace = new Workspaces($userCreds->base_url);
  $workspace->setBasicAuth($userCreds->username, $userCreds->password);
  try {
    $workspace->create($workspace_name);
  } catch (HTTPRequestException $e) {
    $error = $e->getCustomMessage();
    drupal_set_message($error, "error");
  }
  drupal_set_message("Successfully Created Workspace");
}
