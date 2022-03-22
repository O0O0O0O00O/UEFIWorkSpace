extern unsigned char WifiConnectionManagerDxeStrings[];
extern EFI_GUID { 0x9f94d327, 0x0b18, 0x4245, { 0x8f, 0xf2, 0x83, 0x2e, 0x30, 0xd, 0x2c, 0xef }};
#pragma pack(1)
typedef struct _WIFI_MANAGER_IFR_NVDATA {
  UINT32 ProfileCount;
  CHAR16 SSId[33];
  CHAR16 Password[64];
  CHAR16 PrivateKeyPassword[64];
  CHAR16 EapIdentity[64];
  CHAR16 EapPassword[64];
  UINT8 SecurityType;
  UINT8 EapAuthMethod;
  UINT8 EapSecondAuthMethod;
  UINT8 HiddenNetworkList[255];
} WIFI_MANAGER_IFR_NVDATA;
#pragma pack()
formset
  guid = { 0x9f94d327, 0x0b18, 0x4245, { 0x8f, 0xf2, 0x83, 0x2e, 0x30, 0xd, 0x2c, 0xef } },
  title = STRING_TOKEN(0x0002),
  help = STRING_TOKEN(0x0003),
  class = 0x04,
  subclass = 0x03,
  varstore WIFI_MANAGER_IFR_NVDATA,
    varid = 0x0802,
    name = WIFI_MANAGER_IFR_NVDATA,
    guid = { 0x9f94d327, 0x0b18, 0x4245, { 0x8f, 0xf2, 0x83, 0x2e, 0x30, 0xd, 0x2c, 0xef } };
  form formid = 1,
    title = STRING_TOKEN(0x0004);
    suppressif TRUE;
      text
        help = STRING_TOKEN(0x000A),
        text = STRING_TOKEN(0x000A),
        flags = INTERACTIVE,
        key = 0x100;
      endif;
    label 0x1000;
    label 0xffff;
  endform;
  form formid = 2,
    title = STRING_TOKEN(0x000B);
    text
      help = STRING_TOKEN(0x0007),
      text = STRING_TOKEN(0x0005),
        text = STRING_TOKEN(0x0006);
    text
      help = STRING_TOKEN(0x000A),
      text = STRING_TOKEN(0x0008),
        text = STRING_TOKEN(0x0009);
    subtitle text = STRING_TOKEN(0x000A);
    subtitle text = STRING_TOKEN(0x000A);
    goto 3,
         prompt = STRING_TOKEN(0x000C),
         help = STRING_TOKEN(0x000D),
         flags = INTERACTIVE,
         key = 0x102;
    goto 9,
         prompt = STRING_TOKEN(0x003D),
         help = STRING_TOKEN(0x003E),
         flags = INTERACTIVE,
         key = 0x104;
    action
         questionid = 0x101,
         prompt = STRING_TOKEN(0x000A),
         help = STRING_TOKEN(0x000A),
         flags = INTERACTIVE,
         config = STRING_TOKEN(0x000A),
         refreshguid = { 0xde609972, 0xcbcc, 0x4e82, { 0x8b, 0x3e, 0x6a, 0xc5, 0xcf, 0x56, 0x73, 0x8d } },
    endaction;
  endform;
  form formid = 3,
    title = STRING_TOKEN(0x000C);
    numeric varid = WIFI_MANAGER_IFR_NVDATA.ProfileCount,
            prompt = STRING_TOKEN(0x0024),
            help = STRING_TOKEN(0x0025),
            flags = INTERACTIVE | READ_ONLY,
            key = 0x103,
            minimum = 0,
            maximum = 0xffffffff,
            step = 0,
            default = 0,
            refreshguid = { 0xc5f3c7f9, 0xfb9d, 0x49f1, { 0xbe, 0x67, 0x8b, 0xad, 0x20, 0xa7, 0xc6, 0xac } },
    endnumeric;
    subtitle text = STRING_TOKEN(0x000A);
    label 0x2000;
    label 0xffff;
  endform;
  form formid = 4,
    title = STRING_TOKEN(0x000E);
    subtitle text = STRING_TOKEN(0x000A);
    text
      help = STRING_TOKEN(0x0020),
      text = STRING_TOKEN(0x001F),
        text = STRING_TOKEN(0x0021);
    subtitle text = STRING_TOKEN(0x000A);
    text
      help = STRING_TOKEN(0x0011),
      text = STRING_TOKEN(0x000F),
        text = STRING_TOKEN(0x0010);
    text
      help = STRING_TOKEN(0x0014),
      text = STRING_TOKEN(0x0012),
        text = STRING_TOKEN(0x0013);
    suppressif NOT ideqval WIFI_MANAGER_IFR_NVDATA.SecurityType == 4;
      password varid = WIFI_MANAGER_IFR_NVDATA.Password,
                prompt = STRING_TOKEN(0x001D),
                help = STRING_TOKEN(0x001E),
                flags = INTERACTIVE,
                key = 0x201,
                minsize = 8,
                maxsize = 63,
      endpassword;
    endif;
    suppressif NOT ideqval WIFI_MANAGER_IFR_NVDATA.SecurityType == 2;
      oneof varid = WIFI_MANAGER_IFR_NVDATA.EapAuthMethod,
            questionid = 0x204,
            prompt = STRING_TOKEN(0x0015),
            help = STRING_TOKEN(0x0016),
            flags = INTERACTIVE,
            option text = STRING_TOKEN(0x001A), value = 0, flags = DEFAULT;
            option text = STRING_TOKEN(0x001B), value = 1, flags = 0;
            option text = STRING_TOKEN(0x0019), value = 2, flags = 0;
      endoneof;
      suppressif NOT ideqvallist WIFI_MANAGER_IFR_NVDATA.EapAuthMethod == 2
                                                                          0
                                                                          1;
        goto 5,
           prompt = STRING_TOKEN(0x002F),
           help = STRING_TOKEN(0x0030),
           flags = INTERACTIVE,
           key = 0x206;
        suppressif NOT ideqval WIFI_MANAGER_IFR_NVDATA.EapAuthMethod == 2;
            goto 5,
               prompt = STRING_TOKEN(0x0031),
               help = STRING_TOKEN(0x0032),
               flags = INTERACTIVE,
               key = 0x207;
            goto 7,
               prompt = STRING_TOKEN(0x0033),
               help = STRING_TOKEN(0x0034),
               flags = INTERACTIVE,
               key = 0x208;
        endif;
        suppressif NOT ideqvallist WIFI_MANAGER_IFR_NVDATA.EapAuthMethod == 0
                                                                            1;
            oneof varid = WIFI_MANAGER_IFR_NVDATA.EapSecondAuthMethod,
                  questionid = 0x205,
                  prompt = STRING_TOKEN(0x0017),
                  help = STRING_TOKEN(0x0018),
                  flags = INTERACTIVE,
                  option text = STRING_TOKEN(0x001C), value = 0, flags = DEFAULT;
            endoneof;
        endif;
        string varid = WIFI_MANAGER_IFR_NVDATA.EapIdentity,
                prompt = STRING_TOKEN(0x0026),
                help = STRING_TOKEN(0x0027),
                flags = INTERACTIVE,
                key = 0x209,
                minsize = 6,
                maxsize = 63,
        endstring;
        suppressif NOT ideqvallist WIFI_MANAGER_IFR_NVDATA.EapAuthMethod == 0
                                                                            1;
            password varid = WIFI_MANAGER_IFR_NVDATA.EapPassword,
                      prompt = STRING_TOKEN(0x0028),
                      help = STRING_TOKEN(0x0029),
                      flags = INTERACTIVE,
                      key = 0x210,
                      minsize = 0,
                      maxsize = 63,
            endpassword;
        endif;
      endif;
    endif;
    subtitle text = STRING_TOKEN(0x000A);
    text
      help = STRING_TOKEN(0x0023),
      text = STRING_TOKEN(0x0022),
      flags = INTERACTIVE,
      key = 0x202;
    action
      questionid = 0x203,
      prompt = STRING_TOKEN(0x000A),
      help = STRING_TOKEN(0x000A),
      flags = INTERACTIVE,
      config = STRING_TOKEN(0x000A),
      refreshguid = { 0xe5faf2b2, 0x5ecc, 0x44ac, { 0x91, 0x75, 0xfb, 0x78, 0xb2, 0x8a, 0x59, 0x6c } },
    endaction;
  endform;
  form formid = 5,
    title = STRING_TOKEN(0x002E);
    goto 5,
         prompt = STRING_TOKEN(0x0035),
         help = STRING_TOKEN(0x0036),
         flags = INTERACTIVE,
         key = 0x301;
    text
      help = STRING_TOKEN(0x000A),
      text = STRING_TOKEN(0x003A),
      flags = INTERACTIVE,
      key = 0x308;
    subtitle text = STRING_TOKEN(0x000A);
    text
      help = STRING_TOKEN(0x002B),
      text = STRING_TOKEN(0x002A),
      flags = INTERACTIVE,
      key = 0x303;
    text
      help = STRING_TOKEN(0x002D),
      text = STRING_TOKEN(0x002C),
      flags = INTERACTIVE,
      key = 0x304;
  endform;
  form formid = 7,
    title = STRING_TOKEN(0x0033);
    goto 7,
         prompt = STRING_TOKEN(0x0037),
         help = STRING_TOKEN(0x0038),
         flags = INTERACTIVE,
         key = 0x302;
    text
      help = STRING_TOKEN(0x000A),
      text = STRING_TOKEN(0x003B),
      flags = INTERACTIVE,
      key = 0x309;
    subtitle text = STRING_TOKEN(0x000A);
    password varid = WIFI_MANAGER_IFR_NVDATA.PrivateKeyPassword,
              prompt = STRING_TOKEN(0x0039),
              help = STRING_TOKEN(0x000A),
              flags = INTERACTIVE,
              key = 0x307,
              minsize = 0,
              maxsize = 63,
    endpassword;
    subtitle text = STRING_TOKEN(0x000A);
    subtitle text = STRING_TOKEN(0x000A);
    text
      help = STRING_TOKEN(0x002B),
      text = STRING_TOKEN(0x002A),
      flags = INTERACTIVE,
      key = 0x305;
    text
      help = STRING_TOKEN(0x002D),
      text = STRING_TOKEN(0x002C),
      flags = INTERACTIVE,
      key = 0x306;
  endform;
  form formid = 9,
    title = STRING_TOKEN(0x003C);
    subtitle text = STRING_TOKEN(0x000A);
    goto 10,
         prompt = STRING_TOKEN(0x0040),
         help = STRING_TOKEN(0x0041),
         flags = INTERACTIVE,
         key = 0x401;
  endform;
  form formid = 10,
    title = STRING_TOKEN(0x003F);
    string
      varid = WIFI_MANAGER_IFR_NVDATA.SSId,
      prompt = STRING_TOKEN(0x000F),
      help = STRING_TOKEN(0x0011),
      flags = INTERACTIVE,
      minsize = 1,
      maxsize = 32,
    endstring;
    text
      help = STRING_TOKEN(0x0042),
      text = STRING_TOKEN(0x0043),
      flags = INTERACTIVE,
      key = 0x402;
    subtitle text = STRING_TOKEN(0x000A);
    subtitle text = STRING_TOKEN(0x0044);
    label 0x4000;
    label 0xffff;
    text
      help = STRING_TOKEN(0x0045),
      text = STRING_TOKEN(0x0046),
      flags = INTERACTIVE,
      key = 0x403;
  endform;
endformset;
