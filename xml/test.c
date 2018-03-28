#include <stdio.h>
#include "xml.h"

int main(int argc,char** argv)
{
  void* handle=0,* node=0;
  char  buf[260]={0};
  //_track_xml_output("test.xml");
  if(argc==2){
    _track_xml_output(argv[1]);
  } else {
    if(handle=xml_init_document("test.xml")){
      if(xml_get_element_text(handle,"\\element\\section",buf,sizeof(buf),0x0000)){
        printf("%s\n",buf);
      }
      if(xml_get_element_attributes(handle,"\\element\\items\\num","abc",buf,sizeof(buf),0x0000)){
        printf("%s\n",buf);
      }
      if(xml_set_element_attributes(handle,"\\element\\items\\num","abc","chenxiang",0x0000)){
        printf("done!\n");
      }
      if(node=xml_node_branch_open(handle,"\\element\\items\\/lastone")){
        xml_node_branch_insert(node,"insert-me","name=\"2345\" id=\"234\"",1,0,"this line added by TEST");
      }
      if(node=xml_node_branch_open(handle,"\\element\\/test")){
        node=xml_node_branch_insert(node,"insert-sub",0,0,0,0);
        node=xml_node_branch_insert_child(node,"child1","name=\"23\"",1,0,0);
        node=xml_node_branch_insert(node,"child2","name=\"34\" id=\"23333\"",1,0,0);
        node=xml_node_branch_insert(node,"child3","name=\"45\"",1,0,0);
        node=xml_node_branch_insert(node,"/insert-stub",0,0,0,0);
      }
      if(node=xml_node_branch_open(handle,"\\element\\items\\abcdefg")){
        if(xml_get_element_text(node,0,buf,sizeof(buf),0x0001)){
          printf("%s\n",buf);
        }
      }
      if(node=xml_node_branch_open(handle,"\\element\\items\\abcdefg")){
        if(node=xml_node_branch_go_previous(node)){
          if(xml_get_element_attributes(node,0,"str",buf,sizeof(buf),0x0001)){ // located in <singleline .../>
            printf("%s\n",buf);
          }
        }
      }
      if(xml_set_element_text(handle,"\\element\\section","this is changed by TEST",0x0000));
      xml_write_document(handle,"tx.xml"); // write this xml-tree into file. the xml-tree would been destroyed after "xml_free_document()" is called.
                                           // optionally , you could call "xml_update_document()" to write the xml-tree into original file.
      xml_free_document(handle);
    }
  }
  return 0;
}
