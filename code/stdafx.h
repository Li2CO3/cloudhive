#ifndef STDAFX_H
#define STDAFX_H
#pragma once

#include <QString>
#include <iostream>

//#define emit X){//qDebug()<<#X; emit X;}
//#define emit X) emit X//造成了一堆warning
#define __VERSION QString("v20231230")
#define QN(X) QString::number(X)

#endif // STDAFX_H
