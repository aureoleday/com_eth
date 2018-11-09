#ifndef CONFINI_H_
#define CONFINI_H_ 
 
//只有两个接口
int read_conf_value(const char *key, char *value, const char *file);
int write_conf_value(const char *key, char *value, const char *file);

#endif //end of CONFINI_H_
