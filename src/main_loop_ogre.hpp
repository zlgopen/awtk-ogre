/**
 * File:   main_loop_ogre.hpp
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

#ifndef TK_MAIN_LOOP_OGRE_H
#define TK_MAIN_LOOP_OGRE_H

#include "base/lcd.h"
#include "base/main_loop.h"
#include "ogre_types_def.hpp"

BEGIN_C_DECLS

/**
 * @class main_loop_ogre_t
 * @annotation ["fake"]
 * ogre main loop
 */

/**
 * @method main_loop_ogre_init
 * @annotation ["constructor"]
 * @param {ApplicationContext*} app
 * @return {main_loop_t*}
 */
main_loop_t* main_loop_ogre_init(ApplicationContext* app);

END_C_DECLS

#endif /*TK_MAIN_LOOP_OGRE_H*/
