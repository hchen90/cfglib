#include <iostream>
#include "xml.hxx"

int main(int argc,char** argv)
{
  CfgLib_XmlX xml;
  if(xml.init("test.xml")){
    char buf[260]={0};
    if(xml.get_text("element\\test",buf,sizeof(buf))){
      std::cout<<buf<<std::endl;
    }
    void* node=0;
    if(node=xml.open("element\\items\\dir")){
      if(xml.get_text(node,buf,sizeof(buf))){
        std::cout<<buf<<std::endl;
      }
    }
    xml.free();
  }
  return 0;
}

