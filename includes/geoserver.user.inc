<?php

$modulePath = drupal_get_path("module", "geoserver");
require_once  "$modulePath/includes/functions.php";

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
  return get_use_cred();
}
