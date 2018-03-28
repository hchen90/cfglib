#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_section_string(char* ,char* ,char* ,int ,char*);
int set_section_string(char* ,char* ,char* ,char*);

/*
 *  you can include 'section.h',without declaring the two functions.
 * */

int main()
{
  char buf[260]={0};
  if(get_section_string("Sec00","hello",buf,sizeof(buf),"test.cfg")){
    printf("[Sec00]\nhello=%s\n",buf);
  }
  if(get_section_string("Sc123","linux",buf,sizeof(buf),"test.cfg")){
    printf("[Sc123]\nlinux=%s\n",buf);
  }
  if(set_section_string("KKs","SSk","Test","test.cfg")){
    printf("successful!\n");
  }
  if(get_section_string("KKs","SSk",buf,sizeof(buf),"test.cfg")){
    printf("[KKs]\nSSk=%s\n",buf);
  }
  return 0;
}

