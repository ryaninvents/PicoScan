#include <QtGui/QApplication>
#include "mainwindow.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>

#include "hardware/camera/camera.h"
#include "hardware/camera/opencvcamera.h"
#include "hardware/standards/calibrationstandard.h"
#include "hardware/standards/chessboardstandard.h"
#include "hardware/projector/projectionscreen.h"
/*!
\mainpage Scan Studio

Scan Studio allows users to quickly and easily scan 3D objects into the
industry-standard STL format.

\section Attribution

\subsection Oxygen icon set

<a href="http://www.oxygen-icons.org/">Oxygen</a> icon set used under the
terms of the <a href="http://creativecommons.org/licenses/by-sa/3.0/">
CC BY-SA 3.0</a> license.

\subsection Qt

<a href="http://qt-project.org/">Qt</a> application framework used under the
terms of the <a href="http://www.gnu.org/licenses/lgpl-3.0-standalone.html">
GNU Lesser General Public License</a>.

\subsection OpenCV

<a href="http://opencv.org/">OpenCV</a> computer vision library used under the
terms of the <a href="http://opensource.org/licenses/BSD-2-Clause">BSD 2-Clause
License</a>.

\section License

\legalese
Copyright &copy; 2013, Ryan Muller and Chris Thomas.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
\list
\li Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
\li Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
\endlist

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

\endlegalese
*/

#include "test/testmesher.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();

}
