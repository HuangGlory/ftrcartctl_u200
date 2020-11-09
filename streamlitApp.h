#ifndef STREAMLITAPP_H
#define STREAMLITAPP_H

#define STREAMLITAPP_FILE_NAME tr("/home/pi/ftrCartCtl/streamlitApp")

#define STREAMLITAPP tr(\
"#!/usr/bin/python3\n\
# -*- coding: utf-8 -*-\n\
# @Project  :streamlit UI\n\
# @File     :streamlitApp\n\
# @Author   :Glory.Huang\n\
# @Email    :yivi.huang@ftrsystems.com\n\
# @DateTime :2020/10/30@13:16\n\
# @IDE      :Pycharm\n\
# @Version  :1.0\n\
# 1.0\n\
# 1.first Create\n\
#\n\
\n\
import re\n\
import sys\n\
sys.path.append(\"/home/pi/.local/lib/python3.7/site-packages\")\n\
from streamlit.cli import main\n\
if __name__ == '__main__':\n\
    sys.argv[0] = re.sub(r'(-script\.pyw|\.exe)?$', '', sys.argv[0])\n\
    sys.exit(main())\n")

#endif // STREAMLITAPP_H
