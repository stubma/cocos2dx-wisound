cocos2dx-wisound
================

CocosDenshion doesn't support OGG audio so I decide to port WiSound to cocos2d-x. WiSound is audio engine of WiEngine.
It supports OGG, MP3 and WAV for iOS, Android, MacOSX and Windows.

Requirement
===========
cocos2dx-wisound project refers cocos2d-x project. You must clone cocos2d-x repository to a folder named "cocos2d-x", 
and then clone cocos2dx-wisound in the same parent folder. 

I put some common code to https://github.com/stubma/cocos2dx-common, so also clone it in the same parent folder.
The cocos2dx-common is referred by all my cocos2d-x extensions.

Demo
===========
There is a test project in test folder, it supports iOS and android. If you want other platform, do it yourself.

How to use
===========
For iOS, just drag cocos2dx-wisound project into your project. For Android, just import cocos2dx-wisound module. 
If not clear, refer to demo code.
