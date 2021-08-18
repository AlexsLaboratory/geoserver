<?php

function get_use_cred() {
  return db_query("SELECT geo.username, geo.password, geo.base_url FROM geoserver as geo JOIN geoserver_credential_use as cred ON geo.id = cred.id;")->fetchObject();
}