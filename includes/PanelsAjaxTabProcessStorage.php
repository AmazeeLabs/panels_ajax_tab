<?php
/**
 * @file
 *
 * PHP process level static storage.
 */

/**
 * Class PanelsAjaxTabProcessStorage
 */
class PanelsAjaxTabProcessStorage {

  /**
   * Internal storage.
   *
   * @var array
   */
  private static $storage = array();

  /**
   * Set value.
   *
   * @param string $key
   *  Key.
   * @param mixed $value
   *  Value to store.
   */
  public static function set($key, $value) {
    self::$storage[$key] = $value;
  }

  /**
   * Get the stored value if exist.
   *
   * @param string $key
   *  Key.
   * @param mixed $default
   *  Default value in case value is not set.
   * @return mixed
   */
  public static function get($key, $default = NULL) {
    return self::exist($key) ? self::$storage[$key] : $default;
  }

  /**
   * Check if value is set.
   *
   * @param string $key
   *  Key.
   * @return bool
   */
  public static function exist($key) {
    return array_key_exists($key, self::$storage);
  }

}
