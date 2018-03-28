/*****************************************
 * code.h
 * Copyright(C)2014 Sean Chen
 *
 */


#ifndef _CODE_H_
#define _CODE_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*************************************************************
*
* [Section]     <-- 'Section' is the section.
* Name = String <-- 'Name' is the tag, 'String' is the value.
*
* ; this is coment. ( ';' )
*
*************************************************************/

int get_section_string(char* /*section*/ ,char* /*name*/ ,char* /*buffer*/ ,int /*buffer size*/ ,char* /*filename*/ );
int set_section_string(char* /*section*/ ,char* /*name*/ ,char* /*string*/ ,char* /*filename*/ );
const char* section_version();

#ifdef  __cplusplus
}
#endif

#endif

