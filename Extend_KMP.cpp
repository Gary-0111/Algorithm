void GetNext(char *str)
{
    int i = 0, j, pos, len = strlen(str);
    nxt[0] = len;
    while(str[i] == str[i+1] && i+1 < len) i++;
    nxt[1] = i;
    pos = 1;
    for(i = 2; i < len; i++)
    {
        if(nxt[i - pos] + i < nxt[pos] + pos)
            nxt[i] = nxt[i-pos];
        else
        {
            j = nxt[pos] + pos - i;
            if(j < 0) j = 0;
            while(i+j < len && str[j] == str[i+j]) j++;
            nxt[i] = j;
            pos = i;
        }
    }
}

void EXKMP(char *s1, char *s2)
{
    int i = 0, j, pos, l1 = strlen(s1), l2 = strlen(s2);
    GetNext(s2);
    while(s1[i] == s2[i] && i < l1 && i < l2) i++;
    ex[0] = i;
    pos = 0;
    for(i = 1; i < l1; i++)
    {
        if(nxt[i - pos] + i < ex[pos] + pos)
            ex[i] = nxt[i-pos];
        else
        {
            j = ex[pos] + pos - i;
            if(j < 0) j = 0;
            while(i+j < l1 && j < l2 && s1[j+i] == s2[j]) j++;
            ex[i] = j;
            pos = i;
        }
    }
}
