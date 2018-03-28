#include "xml.h"

class CfgLib_Xml {
public:
  CfgLib_Xml():_handle(0){}
  ~CfgLib_Xml(){}
  //
  void* init(const char* filename)
  {
    return _handle=xml_init_document(filename);
  }
  int free()
  {
    return xml_free_document(_handle);
  }
  int free(void* handle)
  {
    return xml_free_document(handle);
  }
  int get_text(const char* path,const char* buf,int cb)
  {
    return xml_get_element_text(_handle,path,buf,cb,0x0000);
  }
  int get_text(void* node,const char* buf,int cb)
  {
    return xml_get_element_text(node,0,buf,cb,0x0001); // set 'path' to null,because it'll been ingored when set 0x0001 in mode.
  }
  int set_text(const char* path,const char* buf)
  {
    return xml_set_element_text(_handle,path,buf,0x0000);
  }
  int set_text(void* node,const char* buf)
  {
    return xml_set_element_text(node,0,buf,0x0001);
  }
  int get_attributes(const char* path,const char* name,const char* buf,int cb)
  {
    return xml_get_element_attributes(_handle,path,name,buf,cb,0x0000);
  }
  int get_attributes(void* node,const char* name,const char* buf,int cb)
  {
    return xml_get_element_attributes(node,0,name,buf,cb,0x0001);
  }
  int set_attributes(const char* path,const char* name,const char* buf)
  {
    return xml_set_element_attributes(_handle,path,name,buf,0x0000);
  }
  int set_attributes(void* node,const char* name,const char* buf)
  {
    return xml_set_element_attributes(node,0,name,buf,0x0001);
  }
  int update()
  {
    return xml_update_document(_handle);
  }
  int write(const char* filename)
  {
    return xml_write_document(_handle,filename);
  }
protected:
  void* _handle;
private:
};

class CfgLib_XmlX : public CfgLib_Xml {
public:
  CfgLib_XmlX():_node(0){}
  ~CfgLib_XmlX(){}
  //
  void* open(const char* path)
  {
    return _node=xml_node_branch_open(_handle,path);
  }
  void* next()
  {
    return xml_node_branch_go_next(_node);
  }
  void* next(void* node)
  {
    return xml_node_branch_go_next(node);
  }
  void* previous()
  {
    return xml_node_branch_go_previous(_node);
  }
  void* previous(void* node)
  {
    return xml_node_branch_go_previous(node);
  }
  void* child()
  {
    return xml_node_branch_go_child(_node);
  }
  void* child(void* node)
  {
    return xml_node_branch_go_child(node);
  }
  void* insert(const char* tag,const char* attributes,int single,const char* text,const char* comment)
  {
    return xml_node_branch_insert(_node,tag,attributes,single,text,comment);
  }
  void* insert(void* node,const char* tag,const char* attributes,int single,const char* text,const char* comment)
  {
    return xml_node_branch_insert(node,tag,attributes,single,text,comment);
  }
  void* insert_child(const char* tag,const char* attributes,int single,const char* text,const char* comment)
  {
    return xml_node_branch_insert_child(_node,tag,attributes,single,text,comment);
  }
  void* insert_child(void* node,const char* tag,const char* attributes,int single,const char* text,const char* comment)
  {
    return xml_node_branch_insert_child(node,tag,attributes,single,text,comment);
  }
  int remove()
  {
    return xml_node_branch_remove(_node);
  }
  int remove(void* node)
  {
    return xml_node_branch_remove(node);
  }
protected:
  void* _node;
private:
};
