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
      "username" => $record->username,
      l("edit", "admin/geoserver/settings/credentials/edit/" . $record->id) . " | " . l("delete", "admin/geoserver/settings/credentials/delete/" . $record->id)
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