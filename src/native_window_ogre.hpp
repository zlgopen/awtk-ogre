/**
 * File:   native_window_ogre.hpp
 * Author: AWTK Develop Team
 * Brief:  native window ogre
 *
 * Copyright (c) 2024 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2024-04-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_NATIVE_WINDOW_OGRE_H
#define TK_NATIVE_WINDOW_OGRE_H

#include "base/lcd.h"
#include "base/native_window.h"
#include "ogre_types_def.hpp"

BEGIN_C_DECLS

/**
 * @class native_window_ogre_t
 * @annotation ["fake"]
 * ogre native window
 */

/**
 * @method native_window_ogre_init
 * @annotation ["constructor"]
 * @param {RenderWindow*} win
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_ogre_init(RenderWindow* win);

/**
 * @method native_window_ogre_deinit
 * @annotation ["destructor"]
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_ogre_deinit(void);

END_C_DECLS

#endif /*TK_NATIVE_WINDOW_OGRE_H*/
