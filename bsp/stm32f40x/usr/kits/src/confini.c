/*
*��ȡ�����ļ�
*�����ļ������µ���ʽ
* 	IP=192.168.1.8
*	NETMASK=255.255.255.0
*	MAC=12:34:56:78:90:11
*֧��#ע��,����������ʱ���ע��ȫ��ȥ��
*/
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
#ifdef __cplusplus
extern "C"
{
#endif
 
typedef struct item_t {
	char *key;
	char *value;
}ITEM;

/*
*ȥ���ַ����Ҷ˿ո�
*/
char *strtrimr(char *pstr)
{
	int i;
	i = strlen(pstr) - 1;
	while (isspace(pstr[i]) && (i >= 0))
		pstr[i--] = '\0';
	return pstr;
}
/*
*ȥ���ַ�����˿ո�
*/
char *strtriml(char *pstr)
{
	int i = 0, j;
	j = strlen(pstr) - 1;
	while (isspace(pstr[i]) && (i <= j))
		i++;
	if (0<i)
		strcpy(pstr, &pstr[i]);
	return pstr;
}
/*
*ȥ���ַ������˿ո�
*/
char *strtrim(char *pstr)
{
	char *p;
	p = strtrimr(pstr);
	return strtriml(p);
}


/*
*�������ļ���һ�ж���key��value,����itemָ��
*line--�������ļ�������һ��
*/
int  get_item_from_line(char *line, ITEM *item)
{
	char *p = strtrim(line);
	int len = strlen(p);
	if (len <= 0){
		return 1;//����
	}
	else if (p[0] == '#'){
		return 2;
	}
	else{
		char *p2 = strchr(p, '=');
		*p2++ = '\0';
		item->key = (char *)malloc(strlen(p) + 1);
		item->value = (char *)malloc(strlen(p2) + 1);
		strcpy(item->key, p);
		strcpy(item->value, p2);

	}
	return 0;//��ѯ�ɹ�
}

int file_to_items(const char *file, ITEM *items, int *num)
{
	char line[1024];
	FILE *fp;
	fp = fopen(file, "r");
	if (fp == NULL)
		return 1;
	int i = 0;
	while (fgets(line, 1023, fp)){
		char *p = strtrim(line);
		int len = strlen(p);
		if (len <= 0){
			continue;
		}
		else if (p[0] == '#'){
			continue;
		}
		else{
			char *p2 = strchr(p, '=');
			/*������Ϊֻ��keyûʲô����*/
			if (p2 == NULL)
				continue;
			*p2++ = '\0';
			items[i].key = (char *)malloc(strlen(p) + 1);
			items[i].value = (char *)malloc(strlen(p2) + 1);
			strcpy(items[i].key, p);
			strcpy(items[i].value, p2);

			i++;
		}
	}
	(*num) = i;
	fclose(fp);
	return 0;
}

/*
*��ȡvalue
*/
int read_conf_value(const char *key, char *value, const char *file)
{
	char line[1024];
	FILE *fp;
	fp = fopen(file, "r");
	if (fp == NULL)
		return 1;//�ļ��򿪴���
	while (fgets(line, 1023, fp)){
		ITEM item;
		get_item_from_line(line, &item);
		if (!strcmp(item.key, key)){
			strcpy(value, item.value);

			fclose(fp);
			free(item.key);
			free(item.value);
			break;
		}
	}
	return 0;//�ɹ�
}

int write_conf_value(const char *key, char *value, const char *file)
{
	ITEM items[20];// �ٶ������������20��
	int num;//�洢���ļ���ȡ����Ч��Ŀ
	file_to_items(file, items, &num);

	int i;
	/*for(i=0;i<num;i++){
	printf("key:%s,value:%s\n",items[i].key,items[i].value);
	}*/

	//����Ҫ�޸ĵ���
	for (i = 0; i<num; i++){
		if (!strcmp(items[i].key, key)){
			memset(items[i].value, 0, sizeof(items[i].value));
			strcpy(items[i].value, value);
			//printf("get value(write):%s\n",items[i].value);
			break;
		}
	}

	// ���������ļ�,Ӧ���б��ݣ�����Ĳ����Ὣ�ļ��������
	FILE *fp;
	fp = fopen(file, "w");
	if (fp == NULL)
		return 1;

	for (i = 0; i<num; i++){
		fprintf(fp, "%s=%s\n", items[i].key, items[i].value);
		//printf("%s=%s\n",items[i].key, items[i].value);
	}
	fclose(fp);
	//�������
	for (i = 0; i<num; i++){
		free(items[i].key);
		free(items[i].value);
	}
	return 0;
}
#ifdef __cplusplus
}; //end of extern "C" {
#endif
