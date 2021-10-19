#include <stdio.h>
#include <time.h>
#include "funcs.h"

#define LEN 1234

/* 1 */


int task01(char *nameA, char *nameB, char* s)
{
	FILE *fpA;
	FILE *fpB;
	char buf[LEN];
	int i = 0;
	int j = 0;
	int k = 0;
	int counter = 0;
	
	if (!(fpA = fopen(nameA, "r"))) { return -1; } /* не открылся файл А*/

	if (!(fpB = fopen(nameB, "w"))) { fclose(fpA); return -2; } /* не открылся файл B*/ 
	
	
	while (fgets(buf, LEN, fpA))
	{
		for (i = 0; buf[i]; i++) /* считываем строку в буфер */
		{
			if (buf[i] == '\n')
			{
				buf[i] = 0; 
				break;
			}
		}
	
		k = 0;
		j = 0;
		
		for (; s[j] && buf[k];)
		{
			if (s[j] == '\\' && s[j+1] == '.')
				if (buf[k] != '.')
					break;
				else
					{ k++; j+=2; }
			else
				if (s[j] == '\\' && s[j+1] == '\\')
					if (buf[k] != '\\')
						break;
					else
						{ k++; j+=2; }
				else
					if (s[j] == '.')
						{ k++; j++; }
					else
						if (s[j] != buf[k])
							break;
						else
							{ k++; j++; }
		}
				
		if (!buf[k] && !s[j])
		{
			counter++;
			fprintf(fpB, "%s\n", buf);
		}
	
	}	
	
	if (!(feof(fpA)))
	{
		fclose(fpA);
		fclose(fpB);	
		return -3;
	}
	
	fclose(fpA);
	fclose(fpB);	
	return counter;
}

/* 2 */


int task02(char *nameA, char *nameB, char* s)
{
	FILE *fpA;
	FILE *fpB;
	char buf[LEN];
	int i = 0;
	int j = 0;
	int k = 0;
	int counter = 0;
	
	if (!(fpA = fopen(nameA, "r"))) { return -1; } /* не открылся файл А*/

	if (!(fpB = fopen(nameB, "w"))) { fclose(fpA); return -2; } /* не открылся файл B*/ 
	
	
	while (fgets(buf, LEN, fpA))
	{
		for (i = 0; buf[i]; i++) /* считываем строку в буфер */
		{
			if (buf[i] == '\n')
			{
				buf[i] = 0; 
				break;
			}
		}
	
		k = 0;
		j = 0;
		
		for (; s[j] && buf[k];)
		{
			if (s[j] == '\\' && s[j+1] == '?')
				if (s[j+2] == '?')	
					if (buf[k] != '?')
						j+=3;
					else
						{ k++; j+=3; }
				else
					if (buf[k] != '?')
						break;
					else
						{ k++; j+=2; }
			
				
			else
				if (s[j] == '\\' && s[j+1] == '\\')
					if (s[j+2] == '?')	
						if (buf[k] != '\\')
							j+=3;
						else
							{ k++; j+=3; }
					else
						if (buf[k] != '\\')
							break;
						else
							{ k++; j+=2; }
				else
					if (s[j+1] == '?')
						if (buf[k] == s[j])
							{ k++; j+=2; }
						else
							j+=2;	
					else
						if (s[j] != buf[k])
							break;
						else
							{ k++; j++; }
		}
		
		if (!buf[k])
		{
			while (s[j])
			{
				if (s[j] != '\\' && s[j+1] == '?')
							j+=2;
						else
							if (s[j] == '\\' && (s[j+1] == '\\' || s[j+1] == '?') && s[j+2] == '?')
								j+=3;
							else 
								break;
			}	
		}
		
		

		if (!buf[k] && !s[j])
		{
			counter++;
			fprintf(fpB, "%s\n", buf);
		}

	}
	
	if (!(feof(fpA)))
	{
		fclose(fpA);
		fclose(fpB);	
		return -3;
	}
	
	fclose(fpA);
	fclose(fpB);	
	return counter;
}

/* 3 */

int task03(char *nameA, char *nameB, char* s)
{
	FILE *fpA;
	FILE *fpB;
	char buf[LEN];
	int i = 0;
	int j = 0;
	int k = 0;
	int counter = 0;
	
	if (!(fpA = fopen(nameA, "r"))) { return -1; } /* не открылся файл А*/

	if (!(fpB = fopen(nameB, "w"))) { fclose(fpA); return -2; } /* не открылся файл B*/ 
	
	
	while (fgets(buf, LEN, fpA))
	{
		for (i = 0; buf[i]; i++) /* считываем строку в буфер */
		{
			if (buf[i] == '\n')
			{
				buf[i] = 0; 
				break;
			}
		}
	
		k = 0;
		j = 0;
		
		for (; s[j] && buf[k];)
		{
			if (s[j] == '\\' && s[j+1] == '+')
				if (buf[k] != '+')
					break;
				else
					if (s[j+2] == '+')
					{ 
						for (; buf[k] == s[j+1]; k++);
						j+=3;
					}
					else
					{ k++; j+=2; }
			else
				if (s[j] == '\\' && s[j+1] == '\\')
					if (buf[k] != '\\')
						break;
					else
							if (s[j+2] == '+')
						{ 
							for (; buf[k] == s[j+1]; k++);
							j+=3;
						}
						else
						{ k++; j+=2; }
				else
					if (s[j+1] == '+')
						if (buf[k] == s[j])
						{
							for (; buf[k] == s[j]; k++);
							j+=2;
						}	
						else
							break;
							
					else
						if (s[j] != buf[k])
							break;
						else
							{ k++; j++; }
		}


		if (!buf[k] && !s[j])
		{
			counter++;
			fprintf(fpB, "%s\n", buf);
		}

	}
	
	if (!(feof(fpA)))
	{
		fclose(fpA);
		fclose(fpB);	
		return -3;
	}
	
	fclose(fpA);
	fclose(fpB);	
	return counter;
}


/* 4 */

int task04(char *nameA, char *nameB, char* s)
{
	FILE *fpA;
	FILE *fpB;
	char buf[LEN];
	int i = 0;
	int j = 0;
	int k = 0;
	int counter = 0;
	
	if (!(fpA = fopen(nameA, "r"))) { return -1; } /* не открылся файл А*/

	if (!(fpB = fopen(nameB, "w"))) { fclose(fpA); return -2; } /* не открылся файл B*/ 
	
	
	while (fgets(buf, LEN, fpA))
	{
		for (i = 0; buf[i]; i++) /* считываем строку в буфер */
		{
			if (buf[i] == '\n')
			{
				buf[i] = 0; 
				break;
			}
		}
	
		k = 0;
		j = 0;
		
		for (; s[j] && buf[k];)
		{
			if (s[j] == '\\' && s[j+1] == '*')
				if (s[j+2] == '*')
					if (buf[k] != '*')
						j+=3;
					else
					{
						for (; buf[k] == s[j+1]; k++);
						j+=3;
					}					
				else
					if (buf[k] != '*')
						break;
					else
						{ k++; j+=2; }
				
			else
				if (s[j] == '\\' && s[j+1] == '\\')
					if (s[j+2] == '*')
						if (buf[k] != '\\')
							j+=3;
						else
						{
							for (; buf[k] == s[j+1]; k++);
							j+=3;
						}					
					else
						if (buf[k] != '\\')
							break;
						else
							{ k++; j+=2; }
				
				else
					if (s[j+1] == '*')
						if (buf[k] == s[j])
						{
							for (; buf[k] == s[j]; k++);
							j+=2;
						}
						else
							j+=2;
					else
						if (s[j] != buf[k])
							break;
						else
							{ k++; j++; }
				
				
		}		
			
			
			if (!buf[k])
				while (s[j])
				{ 
					if (s[j] != '\\' && s[j+1] == '*')
						j+=2;
					else
						if (s[j] == '\\' && (s[j+1] == '\\' || s[j+1] == '*') && s[j+2] == '*')
							j+=3;
						else 
							break;
				}
			

		if (!buf[k] && !s[j])
		{
			counter++;
			fprintf(fpB, "%s\n", buf);
		}

	}
	
	if (!(feof(fpA)))
	{
		fclose(fpA);
		fclose(fpB);	
		return -3;
	}
	
	fclose(fpA);
	fclose(fpB);	
	return counter;
}


/* 5 */


int task05(char *nameA, char *nameB, char* s)
{
	FILE *fpA;
	FILE *fpB;
	char buf[LEN];
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int n = 0;
	int flag = 0;
	int counter = 0;
	
	if (!(fpA = fopen(nameA, "r"))) { return -1; } /* не открылся файл А*/

	if (!(fpB = fopen(nameB, "w"))) { fclose(fpA); return -2; } /* не открылся файл B*/ 
	
	
	while (fgets(buf, LEN, fpA))
	{
		for (i = 0; buf[i]; i++) /* считываем строку в буфер */
		{
			if (buf[i] == '\n')
			{
				buf[i] = 0; 
				break;
			}
		}
	
		k = 0;
		j = 0;
		flag = 0;
		
		for (; s[j] && buf[k];)
		{
			if (s[j] == '\\' && s[j+1] == '[')
				if (buf[k] != '[')
					break;
				else
					{ k++; j+=2; }
			else
				if (s[j] == '\\' && s[j+1] == '\\')
					if (buf[k] != '\\')
						break;
					else
						{ k++; j+=2; }
				else
					if (s[j] == '\\' && s[j+1] == ']')
						if (buf[k] != ']')
							break;
						else
							{ k++; j+=2; }
					else
						if (s[j] == '[' && s[j+1] == '\\' && (s[j+2] == '\\' || s[j+2] == ']' || s[j+2] == '[') &&
						s[j+3] == '-' && s[j+4] == '\\' && (s[j+5] == '\\' || s[j+5] == ']' || s[j+5] == '[') && s[j+6] == ']')
						{
							n = s[j+2];
							m = s[j+5];
							for (; n <= m; n++)
								if (buf[k] == n)
								{
									flag = 1;
									break; 
								}
							if (flag == 1)
								{ k++; j+=7; }
							else
								break;
						}
						else
							if (s[j] == '[' && s[j+1] != 0 && s[j+2] == '-' && s[j+3] == '\\' &&
							(s[j+4] == '\\' || s[j+4] == ']' || s[j+4] == '[') && s[j+5] == ']')
							{
								n = s[j+1];
								m = s[j+4];
								for (; n <= m; n++)
									if (buf[k] == n)
									{
										flag = 1;
										break; 
									}
								if (flag == 1)
									{ k++; j+=6; }
								else
									break;
							}
							else
								if (s[j] == '[' && s[j+1] == '\\' && (s[j+2] == '\\' || s[j+2] == ']' || s[j+2] == '[') &&
								s[j+3] == '-' && s[j+4] != 0 && s[j+5] == ']')
								{
									n = s[j+2];
									m = s[j+4];
									for (; n <= m; n++)
										if (buf[k] == n)
										{
											flag = 1;
											break; 
										}
									if (flag == 1)
										{ k++; j+=6; }
									else
										break;
								}
								else
									if (s[j] == '[' && s[j+1] != 0 && s[j+2] == '-' && s[j+3] != 0 && s[j+4] == ']')
									{
										n = s[j+1];
										m = s[j+3];
										for (; n <= m; n++)
											if (buf[k] == n)
											{
												flag = 1;
												break; 
											}
										if (flag == 1)
											{ k++; j+=5; }
										else
											break;
									}	
									else
										if (s[j] != buf[k])
											break;
										else
											{ k++; j++; }
		}
	
		if (!buf[k] && !s[j])
		{
			counter++;
			fprintf(fpB, "%s\n", buf);
		}
	
	}	
	
	if (!(feof(fpA)))
	{
		fclose(fpA);
		fclose(fpB);	
		return -3;
	}
	
	fclose(fpA);
	fclose(fpB);	
	return counter;
}

