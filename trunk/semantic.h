/****************************************************/
/* File: semantic.h                                  */
/* Yueq C Compiler					                    */
/* Author : K.C.L                                   */
/* Further Modified by Qi Yue (yueq@cse.buaa.edu.cn)*/
/****************************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _ANALYZE_H_
#define _ANALYZE_H_


void buildSymtab(TreeNode *);


 
void typeCheck(TreeNode *);

#endif
