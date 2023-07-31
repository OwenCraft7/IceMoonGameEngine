#ifndef NODES_H
#define NODES_H

vert leafTestPosition;
int playerLeaf, prevPlayerLeaf, cameraLeaf;
int leafCheck;
bool renderLevel;

void checknode(const node nod);
static inline void checkfront(const node nod);
static inline void checkback(const node nod);

#endif
