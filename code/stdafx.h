﻿#ifndef STDAFX_H
#define STDAFX_H
#pragma once

#include <QString>
#include <iostream>

//#define emit X){//qDebug()<<#X; emit X;}
//#define emit X) emit X//造成了一堆warning
#define __VERSION QString("v0.2.240517")
#define __RES_VERSION QString("v0.2.240517")
#define QN(X) QString::number(X)

#define NewYear2024NianEasy
#endif // STDAFX_H
