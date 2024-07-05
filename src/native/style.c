#include <stdio.h>
#include "style.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


int style_get_cnt(struct style_table_s *table, const char *type)
{
  struct style_table_s *s;
  struct style_pair_s *p;
  s = table;

  
  while(s->type){
    if(!strcmp(type, s->type)){
      int cnt=0;
      p=s->table;
      while(p->attr){
	cnt++;
	p++;
      }
      return cnt;
    }
    s++;
  }
  return 0;
}

struct style_pair_s  *style_get_idx(struct style_table_s *table, const char *type, uint32_t idx)
{
  struct style_table_s *s;
  struct style_pair_s *p;
  s = table;
  
  while(s->type){
    if(!strcmp(type, s->type)){
      int cnt=0;
      p=s->table;
      while(p->attr){
	if(cnt == idx){
	  return p;
	}
	cnt++;
	p++;
      }
      return NULL;
    }
    s++;
  }
  return NULL;
}

struct style_val_s **create_style(struct style_table_s *table, char *type)
{
  int cnt;

  struct style_val_s **ret;
  struct style_pair_s *p;
  cnt = style_get_cnt(table, type);
  if(cnt ==0)
    return NULL;

  ret = (struct style_val_s **)malloc((cnt + 1) * sizeof(struct style_val_s *));
  memset(ret, 0, (cnt + 1) * sizeof(struct style_val_s *));
  for(int i=0; i < cnt; i++){
    ret[i] = (struct style_val_s*)malloc(sizeof(struct style_val_s ));
    memset(ret[i], 0, sizeof(struct style_val_s));
    p = style_get_idx(table, type, i);
    (ret[i])->type = p->attr;
    (ret[i])->imgui_val = p->val;
  }
  return ret;
}


void free_style(struct style_val_s **s)
{
  struct style_val_s **p=NULL;
  
  for(p=s; *p; p++)
    free(*p);
  free(s);
}


struct style_val_s **add_style(struct style_val_s **target, struct style_table_s *table, char *type)
{
  
  struct style_val_s **ret;
  struct style_pair_s *p;
  int ocnt=0;
  int cnt = 0;

  if(target){
    for(ret=target; *ret; ret++){
      if((*ret)){
	ocnt++;
      }
    }
  }

  
  cnt = ocnt + style_get_cnt(table, type);
  if(cnt ==0)
    return NULL;


  ret = (struct style_val_s **)realloc(target, (cnt + 1) * sizeof(struct style_val_s *));

  for(int i=ocnt; i < cnt; i++){
    ret[i] = (struct style_val_s*)malloc(sizeof(struct style_val_s ));
    memset(ret[i], 0, sizeof(struct style_val_s));
    p = style_get_idx(table, type, i-ocnt);
    (ret[i])->type = p->attr;
    (ret[i])->imgui_val = p->val;
  }
  ret[cnt] = NULL;
  return ret;
}
/*  





int main()
{
  struct style_pair_s *pair;
  struct style_val_s **s=NULL;

  s = create_style(color_table, "slider");
  s = add_style(s, color_table, "frame");


  printf("%ld %ld %ld\n", sizeof(struct style_val_s**), sizeof(struct style_val_s*), sizeof(struct style_val_s));
  struct style_val_s **p=NULL;
  for(p=s; *p; p++)
    printf("%p %s %d\n", *p, (*p)->type, (*p)->imgui_val);

  free_style(s);
  
}

*/
