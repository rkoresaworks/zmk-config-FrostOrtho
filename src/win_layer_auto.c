/*
 * アクティブなBLEプロファイルに合わせてWindowsレイヤーをON/OFFする。
 * レイヤー状態は再起動やディープスリープで消えるが、プロファイルは保存され、
 * 再接続時にイベントが飛ぶのでここで復元される。
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/keymap.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static int win_layer_auto_listener(const zmk_event_t *eh) {
    const struct zmk_ble_active_profile_changed *ev = as_zmk_ble_active_profile_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    bool want = ev->index == CONFIG_FROSTORTHO_WIN_BT_PROFILE;
    if (want != zmk_keymap_layer_active(CONFIG_FROSTORTHO_WIN_LAYER)) {
        LOG_DBG("profile %d: windows layer %s", ev->index, want ? "on" : "off");
        if (want) {
            zmk_keymap_layer_activate(CONFIG_FROSTORTHO_WIN_LAYER);
        } else {
            zmk_keymap_layer_deactivate(CONFIG_FROSTORTHO_WIN_LAYER);
        }
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(win_layer_auto, win_layer_auto_listener);
ZMK_SUBSCRIPTION(win_layer_auto, zmk_ble_active_profile_changed);
