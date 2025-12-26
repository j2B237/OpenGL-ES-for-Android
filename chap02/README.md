# INTRODUCTION

This program will take the native application code that you generated in the First Android Native Application and add the necessary code to it in order to run OpenGL ES graphics. Although this application will not produce anything new on the screen it will form the basis of all tutorials to come and will make explaining concepts in the next few tutorials much easier.


## Application Flow

Just before we begin we will revisit the basic application flow and show where the graphics sections of code fit in.

* The application will start and control will be immediately passed to the Activity onCreate Function.
* This function will create an instance of your View class and then will set the display to this View class.
* On creation the View class will call an initialization function. This function will setup the surface to be rendered too. This involves setting an EGLContextFactory, setting an EGLConfiguration and then setting a Renderer.
* Your renderer has two important functions. One is called when the surface size is either created or changed. The second is called every time a new frame is ready to be drawn. These functions typically reference functions that are written in the native side of your application. It does this by using a different class that holds function prototypes of all your native functions.
* The native side then takes over and will render your scene.

## Creating a new project

I used Sublime Text and Terminal because I think it is worthy for master the whole processe. Even if i take me more time to get my project up and running. You are free to use Android studio or Eclipse.


