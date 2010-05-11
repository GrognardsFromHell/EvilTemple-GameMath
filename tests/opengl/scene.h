
#ifndef SCENE_H
#define SCENE_H

bool CreateScene(int w, int h, const char *modelFile);
void DestroyScene();
void DrawScene(double secondsElapsed);
void KeyPressed(int key);

#endif