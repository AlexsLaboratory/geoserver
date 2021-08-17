<?php

/**
 * @throws Exception
 */
function geoserver_admin_list() {
  $output = "";
  $output .= "<h1>Credential Management</h1>";
  $output .= l("Add Credentials", "admin/geoserver/settings/credentials/add");
  $table = [
    "header" => [
      [
        "data" => "Geoserver Credentials Name",
        "width" => "50%"
      ],
      [
        "data" => "Operations",
        "width" => "50%"
      ],
    ],
    "rows" => [],
  ];

  $result = db_query("SELECT * FROM {geoserver}");
  foreach ($result as $record) {
    $table["rows"][] = [
      "name" => $record->name,
      l("edit", "admin/geoserver/settings/credentials/edit/" . $record->id) . " | " . l("delete", "admin/geoserver/settings/credentials/delete/" . $record->id) . " | " . l("set default", "admin/geoserver/settings/credentials/default/" . $record->id)
    ];

    if (empty($table["rows"])) {
      $table["rows"][] = [
        [
          "data" => "None.",
          "colspan" => 2
        ]
      ];
    }
    $output .= theme("table", $table);
  }
  return $output;
}

/**
 * @throws Exception
 */
function geoserver_admin_settings_credentials_form($form, &$form_state) {

  $obj = NULL;
  if (isset($form_state["build_info"]["args"][1])) {
    $obj = db_query("
            SELECT * FROM {geoserver}
            WHERE id=:id", [":id" => $form_state["build_info"]["args"][1]])->fetchObject();
  }

  $form["id"] = [
    "#type" => "hidden",
    "#value" => (isset($obj->id)) ? $obj->id : NULL,
  ];

  if ($form_state["build_info"]["args"][0] == "delete") {
    $table = [
      "header" => NULL,
      "rows" => [
        [
          [
            "data" => "Name",
            "header" => TRUE
          ],
          $obj->name
        ]
      ],
    ];
    $form["info"] = [
      "#type" => "markup",
      "#markup" => "<p>Would you like to delete the following credentials?</p>" . theme("table", $table)
    ];

    $form["name"] = [
      "#type" => "hidden",
      "#value" => $obj->name,
    ];

    $form["delete"] = [
      "#type" => "submit",
      "#value" => "Delete",
      "#name" => "delete",
      "#suffix" => "<span> " . l("or Cancel", "/admin/geoserver/settings") . "</span>",
    ];
  } elseif ($form_state["build_info"]["args"][0] == "default") {
    $table = [
      "header" => NULL,
      "rows" => [
        [
          [
            "data" => "Name",
            "header" => TRUE
          ],
          $obj->name
        ]
      ],
    ];
    $form["info"] = [
      "#type" => "markup",
      "#markup" => "<p>Would you like to set the following credential as default?</p>" . theme("table", $table)
    ];

    $form["name"] = [
      "#type" => "hidden",
      "#value" => $obj->name,
    ];

    $form["default"] = [
      "#type" => "submit",
      "#value" => "Set Default",
      "#name" => "default",
      "#suffix" => "<span> " . l("or Cancel", "/admin/geoserver/settings") . "</span>",
    ];
  } else {
    $form["name"] = [
      "#type" => "textfield",
      "#title" => "Name",
      "#description" => "Accordion group name for side navigation menu.",
      "#default_value" => (isset($obj->name)) ? $obj->name : "",
    ];

    $form["username"] = [
      "#type" => "textfield",
      "#title" => "Username",
      "#description" => "Rank of the group (the lower the rank the higher is displayed in the accordion.",
      "#default_value" => (isset($obj->username)) ? $obj->username : "",
    ];

    $form["password"] = [
      "#type" => "textfield",
      "#title" => "Password",
      "#description" => "Rank of the group (the lower the rank the higher is displayed in the accordion.",
      "#default_value" => (isset($obj->password)) ? $obj->password : "",
    ];

    $form["baseurl"] = [
      "#type" => "textfield",
      "#title" => "Base Url",
      "#description" => "Rank of the group (the lower the rank the higher is displayed in the accordion.",
      "#default_value" => (isset($obj->base_url)) ? $obj->base_url : "",
    ];

    $op = ucwords($form_state["build_info"]["args"][0]);
    if ($op === "Edit") {
      $form["submit"] = [
        "#type" => "submit",
        "#name" => "edit",
        "#value" => "Save Changes"
      ];
    } else {
      $form["submit"] = [
        "#type" => "submit",
        "#name" => "add",
        "#value" => "Add"
      ];
    }


  }
  return $form;
}

/**
 * Group admin form: Submission.
 * URL suffix: ?q=admin/geoserver/settings/credentials/[add|edit|delete][/<id>]
 */
function geoserver_admin_settings_credentials_form_submit($form, &$form_state) {
  $form_state["rebuild"] = FALSE;
  $form_state["redirect"] = "admin/geoserver/settings";

  if ($form_state["triggering_element"]["#name"]== "delete") {
    db_query("DELETE FROM {geoserver} WHERE id=:id", [":id" => $form_state["values"]["id"]]);
    drupal_set_message("Successfully deleted <em>" . $form_state["values"]["name"] . "</em>.");
  } elseif ($form_state["triggering_element"]["#name"] == "default") {
    $record = [
      "id" => $form_state["values"]["id"]
    ];
    drupal_write_record("geoserver_credential_default", $record);
    drupal_set_message("Successfully set <em>" . $form_state["values"]["name"] . "</em> to default.");
  } else {
    $record = [
      "name" => $form_state["values"]["name"],
      "username" => $form_state["values"]["username"],
      "password" => $form_state["values"]["password"],
      "base_url" => $form_state["values"]["baseurl"],
    ];

    if ($form_state["triggering_element"]["#name"] == "add") {
      drupal_write_record("geoserver", $record);
    } else {
      $record["id"] = $form_state["values"]["id"];
      drupal_write_record("geoserver", $record, "id");
    }
  }
}