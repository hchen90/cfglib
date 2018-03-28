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
 * Library's Name: CfgLib (xml)
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
#include <ctype.h>

#include "config.h"

struct _ProNode{
  char   _name[260];
  char   _value[260];
  struct _ProNode* _next;
};

struct _TreeNode{
  struct {
    char   _label[260];
    struct _ProNode* _property;
    int    _flags;
  }_nfo1;
  struct {
    char*  _text;
    char*  _comment;
    char*  _declaration;
    int    _flags;
  }_nfo2;
  struct _TreeNode* _next[4];
};

struct _StackNode{
  struct _TreeNode*  _item;
  struct _StackNode* _next;
};

struct _XmlDocument{
  struct {
    char _filename[260];
    int  _statg;
    int  _cb;
  }_xmlnfo;
  struct _TreeNode*  _tree;
  struct _StackNode* _stack;
};

/////////////////////////////////////////////////////////////////

void* _malloc(int size)
{
  void* _p=malloc(size);
  if(_p){
    memset(_p,0,size);
  }
  return _p;
}

#define _free(_p) {free(_p);}

/////////////////////////////////////////////////////////////////

struct _StackNode* _stack_init()
{
  struct _StackNode* _node=0;
  _node=(struct _StackNode*)_malloc(sizeof(struct _StackNode));
  return _node;
}

int _stack_uninit(struct _StackNode* stack)
{
  if(stack){
    if(stack->_next){
      _stack_uninit(stack->_next);
    }
    _free(stack);
  }
  return 0;
}

int _stack_push(struct _StackNode* stack,struct _TreeNode* item)
{
  struct _StackNode* _node=0;
  if(stack){
    if(_node=(struct _StackNode*)_malloc(sizeof(struct _StackNode))){
      _node->_next=stack->_next;
      stack->_next=_node;
      _node->_item=item;
      return 1;
    }
  }
  return 0;
}

int _stack_pop(struct _StackNode* stack)
{
  struct _StackNode* _node=0;
  if(stack){
    if(stack->_next){
      _node=stack->_next->_next;
      _free(stack->_next);
      stack->_next=_node;
    }
  }
  return 0;
}

struct _TreeNode* _stack_get_top(struct _StackNode* stack)
{
  if(stack){
    if(stack->_next){
      return stack->_next->_item;
    }
  }
  return 0;
}

/////////////////////////////////////////////////////////////////

char* _ignore(char* str,int mod)
{
  int i,len=strlen(str);
  for(i=0;i<len;i++){
    if(str[i]==0x20)if(mod&0x0001)continue; // loop when space   [0x0001]
    if(str[i]==0x0a)if(mod&0x0002)continue; // loop when LF      [0x0002]
    if(str[i]==0x0d)if(mod&0x0004)continue; // loop when CR      [0x0004]
    if(str[i]==0x09)if(mod&0x0008)continue; // loop when tab     [0x0008]
    break;
  }
  return &str[i];
}

char* _track(char* str,int mod)
{
  int i,len=strlen(str);
  for(i=0;i<len;i++){
    if(str[i]==0x20)if(mod&0x0001)break; // break when space   [0x0001]
    if(str[i]==0x0a)if(mod&0x0002)break; // break when LF      [0x0002]
    if(str[i]==0x0d)if(mod&0x0004)break; // break when CR      [0x0004]
    if(str[i]==0x09)if(mod&0x0008)break; // break when tab     [0x0008]
    if(str[i]=='<' )if(mod&0x0010)break; // break when '<'     [0x0010]
    if(str[i]=='>' )if(mod&0x0020)break; // break when '>'     [0x0020]
    if(str[i]=='=' )if(mod&0x0040)break; // break when equal   [0x0040]
    if(str[i]=='\"')if(mod&0x0080)break; // break when '"'     [0x0080]
    if(str[i]=='\\')if(mod&0x0100)break; // break when '\'     [0x0100]
  }
  return &str[i];
}

int _cvt2space(char* str,int mod)
{
  int _i,_len=strlen(str);
  for(_i=0;_i<_len;_i++){
    if(str[_i]==0x0a)if(mod&0x0001)str[_i]=0x20; // convert '\n' to space      [0x0001]
    if(str[_i]==0x0d)if(mod&0x0002)str[_i]=0x20; // convert '\r' to space      [0x0002]
    if(str[_i]==0x09)if(mod&0x0004)str[_i]=0x20; // convert tab  to space      [0x0004]
  }
  return 0;
}

int _chomp(char* str,int mod)
{
  int i,len=strlen(str);
  for(i=0;i<len;i++){
    if(str[i]==0x20)if(mod&0x0001)str[i]=0; // remove space [0x0001]
    if(str[i]==0x0a)if(mod&0x0002)str[i]=0; // remove LF    [0x0002]
    if(str[i]==0x0d)if(mod&0x0004)str[i]=0; // remove CR    [0x0004]
    if(str[i]==0x09)if(mod&0x0008)str[i]=0; // remove tab   [0x0008]
    if(str[i]=='<' )if(mod&0x0010)str[i]=0; // remove '<'   [0x0010]
    if(str[i]=='>' )if(mod&0x0020)str[i]=0; // remove '>'   [0x0020]
    if(str[i]=='=' )if(mod&0x0040)str[i]=0; // remove '='   [0x0040]
    if(str[i]=='\"')if(mod&0x0080)str[i]=0; // remove '"'   [0x0080]
    if(str[i]=='\\')if(mod&0x0100)str[i]=0; // remove '\'   [0x0100]
  }
  return 0;
}

////

int _xml_is_this_pattern_a_single_line(char* buf)
{
  int _retn=0,_flags=0,_i,_len=strlen(buf);
  if(_len>2){
    if((buf[1]=='/')||(buf[1]=='?')||(buf[1]=='!'))return 0;
  }
  for(_i=0;_i<_len;_i++){
    if(buf[_i]=='\"'){
      if(_flags&0x0001){
        _flags&=~0x0001;
      } else {
        _flags|= 0x0001;
      }
    } else if((!(_flags&0x0001))&&(buf[_i]=='/')){ // onlly finding '/' when '"' is closed.
      _retn=1;
    }
  }
  return _retn;
}

struct _TreeNode* _xml_get_tree_last_node_by_next0(struct _TreeNode* node)
{
  struct _TreeNode* _node=node;
  while(_node->_next[0])_node=_node->_next[0];
  return _node;
}

int _xml_load_this_node_properties(struct _ProNode** node,char* buf)
{
  char* _p1=buf;
  int _cb=0;
  char _buf[260]={0};
  if(buf){
    if(!isalpha(*buf)) \
    _p1=_track(buf,0x0001|0x0008);
    _p1=_ignore(_p1,0x0001|0x0008);
    if(_cb=strlen(_p1)){
      if(!(*node))*node=(struct _ProNode*)_malloc(sizeof(struct _ProNode));
      strncpy(_buf,_p1,sizeof(_buf));
      _chomp(_buf,0x0001|0x0008|0x0040);
      strncpy((*node)->_name,_buf,260-1);
      _p1=_track(_p1,0x0040); // track '='
      _p1=_track(_p1,0x0080); // track '"'
      _p1++;
      strncpy(_buf,_p1,sizeof(_buf));
      _chomp(_buf,0x0080);
      strncpy((*node)->_value,_buf,260-1);
      _p1=_track(_p1,0x0080);
      _p1++;
      if(strlen(_p1)>0)_xml_load_this_node_properties(&((*node)->_next),_p1);
    }
  }
  return 0;
}

int _xml_append_this_item_to_tree(struct _XmlDocument* xml,char* buf)
{
  int _cb=0;
  char _buf[260]={0};
  struct _TreeNode* _node=0,* _anode=0;
  if((*buf=='<')&&(xml->_stack)&&(xml->_tree)){
    if(buf[1]=='?'){ // declaration.
      //printf("%12.26s: %s\n","Declaration",buf);
      _cb=strlen(buf);
      _node=_stack_get_top(xml->_stack);
      if(_node){
        _anode=_xml_get_tree_last_node_by_next0(_node);
        if(_anode){
          _anode->_next[0]=(struct _TreeNode*)_malloc(sizeof(struct _TreeNode));
          if(_anode->_next[0]){
            _anode->_next[0]->_next[3]=_anode; // save previous
            _anode=_anode->_next[0]; // goto next
            if(_anode->_nfo2._declaration=(char*)_malloc(_cb+2)){
              strncpy(_anode->_nfo2._declaration,buf+1,_cb-2);
              _anode->_nfo2._flags|=0x0001;
            }
          }
        }
      } else {
        if(xml->_tree->_nfo2._declaration)_free(xml->_tree->_nfo2._declaration);
        if(xml->_tree->_nfo2._declaration=(char*)_malloc(_cb+2)){
          strncpy(xml->_tree->_nfo2._declaration,buf+1,_cb-2);
          xml->_tree->_nfo2._flags|=0x0001;
        }
      }
      
    } else if(buf[1]=='!'){ // comment.
      //printf("%12.26s: %s\n","Comment",buf);
      _node=_stack_get_top(xml->_stack);
      if(_node){
        _anode=_xml_get_tree_last_node_by_next0(_node);
        if(_anode){
          _anode->_next[0]=(struct _TreeNode*)_malloc(sizeof(struct _TreeNode));
          if(_anode->_next[0]){
            _anode->_next[0]->_next[3]=_anode; // save previous
            _anode=_anode->_next[0];
            _cb=strlen(buf);
            if(_anode->_nfo2._comment=(char*)_malloc(_cb+2)){
              strncpy(_anode->_nfo2._comment,buf+1,_cb-2);
              _anode->_nfo2._flags|=0x0002;
            }
          }
        }
      } else {
        if(xml->_tree->_nfo2._comment)_free(xml->_tree->_nfo2._comment);
        if(xml->_tree->_nfo2._comment=(char*)_malloc(_cb+2)){
          strncpy(xml->_tree->_nfo2._comment,buf+1,_cb-2);
          xml->_tree->_nfo2._flags|=0x0002;
        }
      }
      
    } else if(buf[1]=='/'){ // endian tag.
      //printf("%12.26s: %s\n","EndOfTag",buf);
      _node=_stack_get_top(xml->_stack);
      if(_node){
        _anode=_xml_get_tree_last_node_by_next0(_node);
        if(_anode){
          _anode->_next[0]=(struct _TreeNode*)_malloc(sizeof(struct _TreeNode));
          if(_anode->_next[0]){
            _anode->_next[0]->_next[3]=_anode; // save previous
            _anode=_anode->_next[0];
            strncpy(_buf,buf+1,260);
            _chomp( _buf,0x0001|0x0002|0x0004|0x0008|0x0020);
            strncpy(_anode->_nfo1._label,_buf,260);
            if(_node->_next[2]&&(xml->_xmlnfo._statg&0x0002)){
              //printf("# %s\n",_node->_next[2]->_nfo1._label);
              _stack_pop(xml->_stack);
              /*{
                _node=_stack_get_top(xml->_stack);
                printf("%s\n",_node->_nfo1._label);
              }*/
            }
          }
        }
      }
      xml->_xmlnfo._statg|=0x0002;
      return 0;
      
    } else {
      //printf("%12.26s: %s\n","StartOfTag",buf);
      if(xml->_xmlnfo._statg&0x0001){ // in this case , we need add current node to the SUBITEM.
        _anode=_stack_get_top(xml->_stack);
        if(_anode){
          _node=_xml_get_tree_last_node_by_next0(_anode);
          if(_node){
            _node->_next[1]=(struct _TreeNode*)_malloc(sizeof(struct _TreeNode)); // add to SUBITEM.
            if(_node->_next[1]){
              _node->_next[1]->_next[2]=_node; // save parent
              _node->_next[1]->_next[3]=_node; // save previous
              _node=_node->_next[1];
              strncpy(_buf,buf+1,260);
              _chomp( _buf,0x0001|0x0002|0x0004|0x0008|0x0020);
              strncpy(_node->_nfo1._label,_buf,260);
              if(_xml_is_this_pattern_a_single_line(buf))_node->_nfo1._flags|=0x0004;
              else {
                _node->_nfo1._flags&=~0x0004;
                _stack_push(xml->_stack,_node);
                //printf("[ push sub! ]\n");
              }
              _xml_load_this_node_properties(&_node->_nfo1._property,buf+strlen(_buf)+1);
            }
          }
        }
      } else { // in this case , we need add current node to the NEXT.
        _anode=_stack_get_top(xml->_stack);
        if(_anode){ // add item to next.
          _node=_xml_get_tree_last_node_by_next0(_anode);
          if(_node){
            //printf("%s\n",_node->_nfo1._label);
            _node->_next[0]=(struct _TreeNode*)_malloc(sizeof(struct _TreeNode)); // add to SUBITEM.
            if(_node->_next[0]){
              _node->_next[0]->_next[3]=_node; // save previous
              _node=_node->_next[0];
              strncpy(_buf,buf+1,260);
              _chomp( _buf,0x0001|0x0002|0x0004|0x0008|0x0020);
              strncpy(_node->_nfo1._label,_buf,260);
              _xml_load_this_node_properties(&_node->_nfo1._property,buf+strlen(_buf)+1);
              if(_xml_is_this_pattern_a_single_line(buf))_node->_nfo1._flags|=0x0004;
              else _node->_nfo1._flags&=~0x0004;
            }
          }
        } else { // this is the first node.
          strncpy(_buf,buf+1,260);
          _chomp(_buf ,0x0001|0x0002|0x0004|0x0008|0x0020);
          strcpy(xml->_tree->_nfo1._label,_buf);
          _stack_push(xml->_stack,xml->_tree); // push top-level node into stack.
          //printf("[ push root! ]\n");
          _xml_load_this_node_properties(&xml->_tree->_nfo1._property,buf+strlen(_buf)+1);
        }
      }
      if(!_xml_is_this_pattern_a_single_line(buf))xml->_xmlnfo._statg|=0x0001;
      return 0;
    }
  } else { // this is a text block.
    //printf("%12.26s: %s\n","NormalText",buf);
    _node=_stack_get_top(xml->_stack);
      if(_node){
        _anode=_xml_get_tree_last_node_by_next0(_node);
        if(_anode){
          _anode->_next[0]=(struct _TreeNode*)_malloc(sizeof(struct _TreeNode));
          if(_anode->_next[0]){
            _anode->_next[0]->_next[3]=_anode; // save previous
            _anode=_anode->_next[0];
            _cb=strlen(buf);
            if(_anode->_nfo2._text=(char*)_malloc(_cb+2)){
              strncpy(_anode->_nfo2._text,buf,_cb);
              _anode->_nfo2._flags|=0x0004;
            }
          }
        }
      }
  }
  if(xml){
    xml->_xmlnfo._statg=0;
  }
  return 0;
}

int _xml_load_this_document_from_mem(struct _XmlDocument* xml,char* mem)
{
  int _cb=0;
  char* _p1=0,* _p2=0;
  if((mem)&&xml){
    _p1=_ignore(mem,0x0001|0x0002|0x0004|0x0008); // ignore space, LF, CR, tab characters.
    if(*_p1=='<'){
      _p2=_track(_p1,0x0020); // search '>'
      if((_cb=_p2-_p1+1)>0){
        _p2=(char*)_malloc(_cb+2);
        if(_p2){
          strncpy(_p2,_p1,_cb);
          _cvt2space(_p2,0x0001|0x0002|0x0004);
          _xml_append_this_item_to_tree(xml,_p2);
          _free(_p2);
          _p1+=_cb;
          _p1=_ignore(_p1,0x0001|0x0002|0x0004|0x0008);
          if(strlen(_p1)){
            _xml_load_this_document_from_mem(xml,_p1);
          }
        }
      }
    } else {
      if(isalpha(*_p1)){
        _p2=_track(_p1,0x0010); // search '<'
        _cb=_p2-_p1;
        if(_cb>0){
          _p2=(char*)_malloc(_cb+2);
          if(_p2){
            strncpy(_p2,_p1,_cb);
            _xml_append_this_item_to_tree(xml,_p2);
            _free(_p2);
            _p1+=_cb;
            _p1=_ignore(_p1,0x0001|0x0002|0x0004|0x0008);
            if(strlen(_p1)){
              _xml_load_this_document_from_mem(xml,_p1);
            }
          }
        }
      }
    }
  }
  return 0;
}

int _xml_load_all_elements(struct _XmlDocument* xml)
{
  int _cb=0;
  void* _pm=0;
  FILE* _fp=0;
  if(xml){
    if(_fp=fopen(xml->_xmlnfo._filename,"rb")){
      fseek(_fp,0,SEEK_END);
      _cb=ftell(_fp);
      fseek(_fp,0,SEEK_SET);
      _cb-=ftell(_fp);
      if(_cb>0){
        if(_pm=_malloc(_cb+2)){
          fread(_pm,sizeof(char),_cb,_fp);
          _xml_load_this_document_from_mem(xml,_pm);
          _free(_pm);
        }
      }
      fclose(_fp);
    }
  }
  return _cb;
}

int _xml_free_all_properties(struct _ProNode* node)
{
  if(node){
    if(node->_next)_xml_free_all_properties(node->_next);
    _free(node);
  }
  return 0;
}

int _xml_free_all_elements(struct _TreeNode* node)
{
  if(node){
    if(node->_nfo1._property    )_xml_free_all_properties(node->_nfo1._property);
    if(node->_nfo2._text        )_free(node->_nfo2._text);
    if(node->_nfo2._comment     )_free(node->_nfo2._comment);
    if(node->_nfo2._declaration )_free(node->_nfo2._declaration);
    if(node->_next[0])_xml_free_all_elements(node->_next[0]);
    if(node->_next[1])_xml_free_all_elements(node->_next[1]);
    _free(node);
  }
  return 0;
}

struct _TreeNode* _xml_tree_init()
{
  struct _TreeNode* _node=(struct _TreeNode*)_malloc(sizeof(struct _TreeNode));
  return _node;
}

int _xml_disp_properties(struct _ProNode* pnd,FILE* fp)
{
  if(pnd){
    fprintf(fp," %s=\"%s\"",pnd->_name,pnd->_value);
    if(pnd->_next)_xml_disp_properties(pnd->_next,fp);
  }
  return 0;
}

int _xml_disp_infos(struct _TreeNode* node,FILE* fp,struct _XmlDocument* xml)
{
  if(node){
    if(node->_nfo2._declaration){
      fprintf(fp,"<%s>\n",node->_nfo2._declaration);
    }
    
    if(strlen(node->_nfo1._label)){
      fprintf(fp,"<");
      if(node->_nfo1._label)fprintf(fp,"%s",node->_nfo1._label);
      if(node->_nfo1._property)_xml_disp_properties(node->_nfo1._property,fp);
      if(node->_nfo1._flags&0x0004)fprintf(fp,"/");
      fprintf(fp,">\n");
    }
    
    if(node->_nfo2._comment){
      fprintf(fp,"<%s>\n",node->_nfo2._comment);
    }
    if(node->_nfo2._text){
      fprintf(fp,"%s",node->_nfo2._text);
    }
    
    if(node->_next[1])_xml_disp_infos(node->_next[1],fp,xml);
    if(node->_next[0])_xml_disp_infos(node->_next[0],fp,xml);
  }
  return 0;
}

////

char* _xml_retrive_a_path_pattern(char* str,char* buf)
{
  char* _p=0;
  int _i,_j,_len=strlen(str);
  if(str&&buf){
    if(*str=='\\')_i=1;
    else _i=0;
    for(_p=&str[_i],_j=0;(_i<_len)&&(str[_i])&&(*_p!='\\');_i++,_p++)buf[_j++]=*_p;
  }
  return _p;
}

int _xml_retrive_this_next_node(struct _TreeNode* node,char* str,struct _TreeNode** nd)
{
  if(node){
    if(!strcmp(node->_nfo1._label,str))*nd=node;
    else if(node->_next[0])_xml_retrive_this_next_node(node->_next[0],str,nd); // retrive the next one if not match this one.
  }
  return 0;
}

int _xml_retrive_this_node(struct _TreeNode* node,char* suf,struct _TreeNode** nd)
{
  char* _p1=suf,_buf[260]={0};
  if(strlen(suf)&&node){
    _p1=_xml_retrive_a_path_pattern(_p1,_buf);
    if(strlen(_buf))_xml_retrive_this_next_node(node,_buf,nd);
    if(strlen(_p1))_xml_retrive_this_node((*nd)->_next[1],_p1,nd);
  }
  return 0;
}

int _xml_retrive_properties_node(struct _ProNode* node,char* name,struct _ProNode** pd)
{
  if(node&&name){
    if(!strcmp(node->_name,name)){
      *pd=node;
    } else if(node->_next) \
    _xml_retrive_properties_node(node->_next,name,pd);
  }
  return 0;
}

/////////////////////////////////////////////////////////////////

const char* xml_version()
{
  return PACKAGE_STRING;
}

void* xml_init_document(const char* filename)
{
  struct _XmlDocument* doc=0;
  if(doc=(struct _XmlDocument*)_malloc(sizeof(struct _XmlDocument))){
    strncpy(doc->_xmlnfo._filename,filename,260);
    doc->_stack=_stack_init();
    doc->_tree=_xml_tree_init();
    _xml_load_all_elements(doc);
    doc->_xmlnfo._cb=sizeof(struct _XmlDocument);
  }
  return doc;
}

int xml_free_document(void* handle)
{
  if(handle){
    if(((struct _XmlDocument*)handle)->_xmlnfo._cb!=sizeof(struct _XmlDocument))return -1;
    if(((struct _XmlDocument*)handle)->_stack) \
      _stack_uninit(((struct _XmlDocument*)handle)->_stack);
    if(((struct _XmlDocument*)handle)->_tree ) \
      _xml_free_all_elements(((struct _XmlDocument*)handle)->_tree);
  }
  return 0;
}

int xml_get_element_text(void* handle,const char* path,const char* buf,int cb,int mode)
{
  int _retn=0;
  struct _XmlDocument* doc=(struct _XmlDocument*)handle;
  struct _TreeNode* _node=0;
  if(doc&&buf){
    if(mode){
      _node=(struct _TreeNode*)handle;
      goto  ___ps_get_element;
    } else {
      if(((struct _XmlDocument*)handle)->_xmlnfo._cb!=sizeof(struct _XmlDocument))return -1;
      _xml_retrive_this_node(doc->_tree,(char*)path,&_node);
___ps_get_element:
      if(_node){
        for(;_node->_next[0];_node=_node->_next[0]){
          if(_node->_nfo2._flags&0x0001)continue; // declaration.
          if(_node->_nfo2._flags&0x0002)continue; // comment.
          if(_node->_nfo2._flags&0x0004){
            strncpy((char*)buf,_node->_nfo2._text,cb);
            _retn=1;
            break;
          }
        }
      }
    }
  }
  return _retn;
}

int xml_set_element_text(void* handle,const char* path,const char* buf,int mode)
{
  int _retn=0,_cb=0;
  struct _XmlDocument* doc=(struct _XmlDocument*)handle;
  struct _TreeNode* _node=0;
  if(doc&&buf){
    if(mode){
      _node=(struct _TreeNode*)handle;
      goto  ___ps_set_element;
    } else {
      if(((struct _XmlDocument*)handle)->_xmlnfo._cb!=sizeof(struct _XmlDocument))return -1;
      _xml_retrive_this_node(doc->_tree,(char*)path,&_node);
___ps_set_element:
      _cb=strlen(buf);
      if(_node){
        for(;_node->_next[0];_node=_node->_next[0]){
          if(_node->_nfo2._flags&0x0001)continue; // declaration.
          if(_node->_nfo2._flags&0x0002)continue; // comment.
          if(_node->_nfo2._text){
            _free(_node->_nfo2._text);
            _node->_nfo2._text=(char*)_malloc(_cb+2);
            strncpy(_node->_nfo2._text,buf,_cb);
            _node->_nfo2._flags|=0x0004;
            _retn=1;
            break;
          }
        }
      }
    }
  }
  return _retn;
}

int xml_get_element_attributes(void* handle,const char* path,const char* name,const char* buf,int cb,int mode)
{
  int _retn=0;
  struct _XmlDocument* doc=(struct _XmlDocument*)handle;
  struct _TreeNode* _node=0;
  struct _ProNode*  _pnd=0;
  if(doc&&name&&buf){
    if(mode){
      _node=(struct _TreeNode*)handle;
      goto  ___ps_get_attrs;
    } else {
      if(((struct _XmlDocument*)handle)->_xmlnfo._cb!=sizeof(struct _XmlDocument))return -1;
      _xml_retrive_this_node(doc->_tree,(char*)path,&_node);
___ps_get_attrs:
      if(_node){
         if(_node->_nfo1._property){
          _xml_retrive_properties_node(_node->_nfo1._property,(char*)name,&_pnd);
          if(_pnd){
            strncpy((char*)buf,_pnd->_value,cb);
            _retn=1;
          }
        }
      }
    }
  }
  return _retn;
}

int xml_set_element_attributes(void* handle,const char* path,const char* name,const char* buf,int mode)
{
  int _retn=0;
  struct _XmlDocument* doc=(struct _XmlDocument*)handle;
  struct _TreeNode* _node=0;
  struct _ProNode*  _pnd=0;
  if(doc&&name&&buf){
    if(mode){
      _node=(struct _TreeNode*)handle;
      goto  ___ps_set_attrs;
    } else {
      if(((struct _XmlDocument*)handle)->_xmlnfo._cb!=sizeof(struct _XmlDocument))return -1;
      _xml_retrive_this_node(doc->_tree,(char*)path,&_node);
___ps_set_attrs:
      if(_node){
         if(_node->_nfo1._property){
          _xml_retrive_properties_node(_node->_nfo1._property,(char*)name,&_pnd);
          if(_pnd){
            strncpy(_pnd->_value,buf,260);
            _retn=1;
          }
        }
      }
    }
  }
  return _retn;
}

int xml_update_document(void* handle)
{
  int _retn=0;
  FILE* _fp=0;
  struct _XmlDocument* doc=(struct _XmlDocument*)handle;
  if(doc){
    if(((struct _XmlDocument*)handle)->_xmlnfo._cb!=sizeof(struct _XmlDocument))return -1;
    if(_fp=fopen(doc->_xmlnfo._filename,"wb")){
      _xml_disp_infos(doc->_tree,_fp,doc);
      fclose(_fp);
      _retn=1;
    }
  }
  return _retn;
}

int xml_write_document(void* handle,const char* filename)
{
  int _retn=0;
  FILE* _fp=0;
  struct _XmlDocument* doc=(struct _XmlDocument*)handle;
  if(doc){
    if(((struct _XmlDocument*)handle)->_xmlnfo._cb!=sizeof(struct _XmlDocument))return -1;
    if(_fp=fopen(filename,"wb")){
      _xml_disp_infos(doc->_tree,_fp,doc);
      fclose(_fp);
      _retn=1;
    }
  }
  return _retn;
}

void* xml_node_branch_open(void* handle,const char* path)
{
  struct _TreeNode* _node=0;
  if(handle&&path){
    _xml_retrive_this_node(((struct _XmlDocument*)handle)->_tree,(char*)path,&_node);
  }
  return _node;
}

void* xml_node_branch_go_next(void* treenode)
{
  struct _TreeNode* _node=0;
  if(treenode){
    _node=((struct _TreeNode*)treenode)->_next[0]; // this is next node.
  }
  return _node;
}

void* xml_node_branch_go_previous(void* treenode)
{
  struct _TreeNode* _node=0;
  if(treenode){
    _node=((struct _TreeNode*)treenode)->_next[3]; // this is previous node.
  }
  return _node;
}

void* xml_node_branch_go_child(void* treenode)
{
  struct _TreeNode* _node=0;
  if(treenode){
    _node=((struct _TreeNode*)treenode)->_next[1]; // this node is child.
  }
  return _node;
}

void* xml_node_branch_insert(void* treenode,const char* tag,const char* attributes,int single,const char* text,const char* comment)
{
  int _cb=0;
  char _buf[260]={0},* _p=0;
  struct _TreeNode* _node[2]={0};
  if(treenode){
    _node[0]=(struct _TreeNode*)treenode;
    _node[1]=(struct _TreeNode*)_malloc(sizeof(struct _TreeNode));
    if(_node[1]){
      if(tag){
        strncpy(_buf,(char*)tag,sizeof(_buf));
        _chomp(_buf,0x0001|0x0002|0x0004|0x0008);
        strncpy(_node[1]->_nfo1._label,_buf,260);
        if(single)_node[1]->_nfo1._flags|=0x0004;
      }
      if(attributes){
        _p=_track((char*)attributes,0x0040);
        if(_p)if(*_p=='=')_xml_load_this_node_properties(&_node[1]->_nfo1._property,(char*)attributes);
      }
      if(text){
        _cb=strlen(text);
        if(_node[1]->_nfo2._text=(char*)_malloc(_cb+2)){
          strncpy(_node[1]->_nfo2._text,text,_cb);
          _node[1]->_nfo2._flags|=0x0004;
        }
      }
      if(comment){
        _cb=strlen(comment);
        if(_node[1]->_nfo2._comment=(char*)_malloc(_cb+8)){
          strncpy(_node[1]->_nfo2._comment,"!--",3);
          strcat(_node[1]->_nfo2._comment,comment);
          strcat(_node[1]->_nfo2._comment,"--");
        }
      }
      _node[1]->_next[0]=_node[0]->_next[0];
      _node[0]->_next[3]=_node[0]; // save previous
      _node[0]->_next[0]=_node[1];
    }
  }
  return _node[1];
}

void* xml_node_branch_insert_child(void* treenode,const char* tag,const char* attributes,int single,const char* text,const char* comment)
{
  int _cb=0;
  char _buf[260]={0},* _p=0;
  struct _TreeNode* _node[2]={0};
  if(treenode){
    _node[0]=(struct _TreeNode*)treenode;
    _node[1]=(struct _TreeNode*)_malloc(sizeof(struct _TreeNode));
    if(_node[1]){
      if(tag){
        strncpy(_buf,(char*)tag,sizeof(_buf));
        _chomp(_buf,0x0001|0x0002|0x0004|0x0008);
        strncpy(_node[1]->_nfo1._label,_buf,260);
        if(single)_node[1]->_nfo1._flags|=0x0004;
      }
      if(attributes){
        _p=_track((char*)attributes,0x0040);
        if(_p)if(*_p=='=')_xml_load_this_node_properties(&_node[1]->_nfo1._property,(char*)attributes);
      }
      if(text){
        _cb=strlen(text);
        if(_node[1]->_nfo2._text=(char*)_malloc(_cb+2)){
          strncpy(_node[1]->_nfo2._text,text,_cb);
          _node[1]->_nfo2._flags|=0x0004;
        }
      }
      if(comment){
        _cb=strlen(comment);
        if(_node[1]->_nfo2._comment=(char*)_malloc(_cb+8)){
          strncpy(_node[1]->_nfo2._comment,"!--",3);
          strcat(_node[1]->_nfo2._comment,comment);
          strcat(_node[1]->_nfo2._comment,"--");
        }
      }
      _node[1]->_next[1]=_node[0]->_next[1];
      _node[0]->_next[2]=_node[0]; // save parent
      _node[0]->_next[3]=_node[0]; // save previous
      _node[0]->_next[1]=_node[1];
    }
  }
  return _node[1];
}

int xml_node_branch_remove(void* treenode)
{
  int _retn=0;
  struct _TreeNode* _node=0;
  if(_node=(struct _TreeNode*)treenode){
    if((_node->_next[3])&&(!_node->_next[2])){ // check previous node
      _node->_next[3]->_next[0]=_node->_next[0];
      _node->_next[0]->_next[3]=_node->_next[3];
      if(_node->_next[1])_xml_free_all_elements(_node->_next[1]);
      _free(_node);
      _retn=1;
    } else if(_node->_next[2]){ // check parent node
      _node->_next[2]->_next[1]=_node->_next[0];
      _node->_next[0]->_next[2]=_node->_next[2];
      _node->_next[0]->_next[3]=_node->_next[2];
      if(_node->_next[1])_xml_free_all_elements(_node->_next[1]);
      _free(_node);
      _retn=1;
    }
  }
  return _retn;
}

/////////////////////////////////////////////////////////////////

#ifndef RM_TRACK

int _track_xml_output(char* filename)
{
  void* _handle=xml_init_document(filename);
  if(_handle){
    _xml_disp_infos(((struct _XmlDocument*)_handle)->_tree,stdout,(struct _XmlDocument*)_handle);
    xml_free_document(_handle);
  }
  return 0;
}

#endif

/****************************************************************
*
* Name: CfgLib (xml)
* Author: Sean Chen
* Bug Report: a.chenxiang.z@gmail.com
*
****************************************************************/
