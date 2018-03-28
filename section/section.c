/**********************************************
 *
 * ReadMe:
 * This library is an open source library.
 * You can redistribute it and/or modify it
 * under the term of General Public License
 * as published by Free Software Foundation.
 * Please see <http://www.gnu.org/licenses/>
 * for more detail.
 *
 * Library's Name: CfgLib (section)
 * Copyright(C)2014 Sean Chen
 *
 **********************************************/

/**********************************************
 *
 * 请阅读：
 * 这是一个开放原码级别的库。
 * 你可以在GPL协议的许可下分发或者改动源代码，
 * GPL协议是由自由软件协会制定的。
 * 可以登陆<http://www.gnu.org/license/>获取该
 * 协议的详细内容。
 *
 **********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"

struct _Node {
  char   _name[260];
  char   _value[260];
  struct _Node* pNext;
};

struct _HashNode {
  char   _section[260];
  struct _Node* pHeader;
  struct _HashNode* pNext;
};

/****************************************************************
 *
 * [Section]        <-- this is section.
 * Name =           <-- "Name" is the name.
 * OtherName = Data <-- "Data" is the string you want to find.
 *
 * ;  this is comment.
 *
 ***************************************************************/

int _free_this_section_names(struct _Node* node)
{
  struct _Node* p1=0,* p2=0;
  if(node){
    p1=node;
    do {
      p2=p1->pNext;
      if(p1)free(p1);
      p1=p2;
    } while(p1);
  }
  return 0;
}

int _free_all_sections(struct _HashNode* hash)
{
  struct _HashNode* p1=0,* p2=0;
  if(hash){
    p1=hash;
    do {
      if(p1->pHeader)_free_this_section_names(p1->pHeader);
      p2=p1->pNext;
      if(p1)free(p1);
      p1=p2;
    } while(p1);
  }
  return 0;
}

int _chomp(char* str,int mod)
{
  int i,len=strlen(str);
  for(i=0;i<len;i++){
    if(str[i]==' ' )if(mod&0x0001)str[i]=0; // remove space [0x0001]
    if(str[i]=='\n')if(mod&0x0002)str[i]=0; // remove LF    [0x0002]
    if(str[i]=='\r')if(mod&0x0004)str[i]=0; // remove CR    [0x0004]
    if(str[i]=='=' )if(mod&0x0008)str[i]=0; // remove equal [0x0008]
    if(str[i]==']' )if(mod&0x0100)str[i]=0; // remove ']'   [0x0100]
    if(str[i]=='[' )if(mod&0x0200)str[i]=0; // remove '['   [0x0200]
    if(str[i]==0x09)if(mod&0x0400)str[i]=0; // remove tab   [0x0400]
    if(str[i]=='/' )if(mod&0x0800)str[i]=0; // remove '/'   [0x0800]
    if(str[i]=='\\')if(mod&0x0010)str[i]=0; // remove '\'   [0x0010]
    if(str[i]=='\"')if(mod&0x0020)str[i]=0; // remove '"'   [0x0020]
    if(str[i]=='\'')if(mod&0x0040)str[i]=0; // remove '''   [0x0040]
  }
  return 0;
}

char* _find_section_name_value(char* str)
{
  char* p=str;
  int i;
  if(str){
    for(i=0;i<260;i++){
      if(str[i]=='='){
        for(i++;(i<260)&&(str[i]==' ');i++);
        p=&str[i];
      }
    }
  }
  return p;
}

int _load_this_section(struct _HashNode* node,char* sec) // add sec into hash table.
{
  struct _HashNode* p=node;
  if(p&&sec){
    for(;p->pNext;p=p->pNext);  // redirect to the last one.
    strncpy(p->_section,sec,260/sizeof(char)-1);
    p->pHeader=0;
    if(p->pNext=(struct _HashNode*)malloc(sizeof(struct _HashNode)))
    memset(p->pNext,0,sizeof(struct _HashNode));
  }
  return 0;
}

struct _HashNode* _load_all_sections(char* filename) // initialize sections.
{
  char  _buf[1024]={0};
  struct _HashNode* hash=0;
  FILE* _fp=fopen(filename,"rb");
  if(_fp){
    if(hash=(struct _HashNode*)malloc(sizeof(struct _HashNode))){
      memset(hash,0,sizeof(struct _HashNode));
      while(!feof(_fp)){
        if(fgets(_buf,sizeof(_buf),_fp)){
          _chomp(_buf,0x02|0x04); // remove LF/CR
          if(_buf[0]==';'){
            continue; // if comment;
          }
          if(_buf[0]=='['){
            _chomp(_buf,0x0100|0x0200); // remove '[' and ']'
            _load_this_section(hash,&_buf[1]);
          }
        }
        memset(_buf,0,sizeof(_buf));
      }
    }
    fclose(_fp);
  }
  return hash;
}

int _add_section_name_to_list(struct _HashNode* hash,char* name,char* value) // add name and value into list.
{
  struct _Node* p=0;
  if(!hash->pHeader){
    if(hash->pHeader=(struct _Node*)malloc(sizeof(struct _Node)))
    memset(hash->pHeader,0,sizeof(struct _Node));
  }
  if(p=hash->pHeader){
    for(;p->pNext;p=p->pNext); // redirect to the last one.
    strncpy(p->_name,name,260/sizeof(char)-1);
    strncpy(p->_value,value,260/sizeof(char)-1);
    if(p->pNext=(struct _Node*)malloc(sizeof(struct _Node)))
    memset(p->pNext,0,sizeof(struct _Node));
  }
  return 0;
}

int _load_this_section_name(struct _HashNode* hash,FILE* fp,char* sec) // add name into name's list.
{
  char _buf[1024]={0},_suf[1024]={0};
  struct _HashNode* p=hash;
  if(p&&fp&&sec){
    while(p){
      if(!strcmp(p->_section,sec))break; // find the proper node.
      p=p->pNext;
    }
    if(!strcmp(p->_section,sec)){
      while(!feof(fp)){
        if(fgets(_buf,sizeof(_buf),fp)){
          _chomp(_buf,0x02|0x04); // remove LF/CR
          if(_buf[0]=='['){
            break;  // if reach the end of this section.
          }
          if(_buf[0]==';'){
            continue; // if comment.
          }
          if(_buf[0]){
            strcpy(_suf,_buf);
            _chomp(_suf,0x01|0x08|0x0400); // remove equal, space and tab
            _add_section_name_to_list(p,_suf,_find_section_name_value(_buf));
          }
        }
        memset(_buf,0,sizeof(_buf));
        memset(_suf,0,sizeof(_suf));
      }
    }
  }
  return 0;
}

int _load_all_names(struct _HashNode* hash,char* filename) // initialize sections' name.
{
  long _pos=0;
  char _buf[260]={0};
  FILE* _fp=fopen(filename,"rb");
  if(_fp){
    while(fgets(_buf,sizeof(_buf),_fp)){
      _chomp(_buf,0x02|0x04); // remove LF/CR
      if(_buf[0]=='['){
        _chomp(_buf,0x0100|0x0200); // remove '[' and ']'
        _pos=ftell(_fp);
        _load_this_section_name(hash,_fp,&_buf[1]);
        fseek(_fp,_pos,SEEK_SET);
      }
    }
    fclose(_fp);
  }
  return 0;
}

int get_section_string(char* sec,char* name,char* buf,int cb,char* filename)
{
  int    _retn=0;
  struct _HashNode* hash=_load_all_sections(filename);
  struct _HashNode* p=hash;
  struct _Node*     pn=0;
  if(hash){
    _load_all_names(hash,filename);
    for(;p;p=p->pNext){
      if(!strcmp(p->_section,sec)){ // if found the section.
        if(pn=p->pHeader){
          for(;pn;pn=pn->pNext){
            if(!strcmp(pn->_name,name)){ // if found the name.
              //printf("\t%s:%s\n",pn->_name,pn->_value);
              strncpy(buf,pn->_value,cb);
              _retn=1;
            }
          }
        }
      }
    }
    _free_all_sections(hash);
  }
  return _retn;
}

int _add_new_item_to_names_list(struct _Node* node,char* name,char* value)
{
  struct _Node* p=0;
  if(!(node)){
    if(node=(struct _Node*)malloc(sizeof(struct _Node)))
    memset(node,0,sizeof(sizeof(struct _Node)));
  }
  p=node;
  for(;p;p=p->pNext){ // if name already existed.
    if(!strcmp(p->_name,name)){
      strncpy(p->_value,value,260/sizeof(char)-1);
      return 0;
    }
  }
  p=node;
  for(;p->pNext;p=p->pNext);  // redirection.
  strncpy(p->_name,name,260/sizeof(char)-1);
  strncpy(p->_value,value,260/sizeof(char)-1);
  if(p->pNext=(struct _Node*)malloc(sizeof(struct _Node)))
  memset(p->pNext,0,sizeof(struct _Node));
  return 0;
}

int _add_new_section_to_table(struct _HashNode* hash,char* sec)
{
  struct _HashNode* p=hash;
  if(hash){
    for(;p->pNext;p=p->pNext);
    strncpy(p->_section,sec,260/sizeof(char)-1);
    p->pHeader=0;
    if(p->pNext=(struct _HashNode*)malloc(sizeof(struct _HashNode)))
    memset(p->pNext,0,sizeof(struct _HashNode));
  }
  return 0;
}

int set_section_string(char* sec,char* name,char* buf,char* filename)
{
  int    _retn=0;
  struct _HashNode* hash=_load_all_sections(filename);
  struct _HashNode* p=hash;
  struct _Node*     pn=0;
  FILE*  _fp=0;
  if(hash){
    _load_all_names(hash,filename);
    for(;p;p=p->pNext){
      if(!strcmp(p->_section,sec)){ // if found the section
        _add_new_item_to_names_list(p->pHeader,name,buf); // add new name and value into list.
        _retn=1;
      }
    }
    if(!_retn){
      _add_new_section_to_table(hash,sec); // section not on table, we need append it.
      p=hash;
      for(;p;p=p->pNext){
        if(!strcmp(p->_section,sec)){ // found section.
          _add_new_item_to_names_list(p->pHeader,name,buf); // add new item.
          _retn=1;
        }
      }
    }
    p=hash;
    if(_fp=fopen(filename,"wb")){
      for(;p;p=p->pNext){
        if(strcmp(p->_section,"")){ // if this node is empty.
          fprintf(_fp,"[%s]\n",p->_section);
          for(pn=p->pHeader;pn;pn=pn->pNext){
            if(strcmp(pn->_name,""))fprintf(_fp,"%s=%s\n",pn->_name,pn->_value);
          }
        }
      }
      fclose(_fp);
    }
    _free_all_sections(hash);
  }
  return _retn;
}

const char* section_version()
{
  return PACKAGE_STRING;
}

/****************************************************************
*
* Name: CfgLib (section)
* Author: Sean Chen
* Bug Report: a.chenxiang.z@gmail.com
*
****************************************************************/
