#pragma once

#include "TreeNode.h"

enum rebalanceCase { case1, case2, case3, case4, case5 };

class RedBlackTree {
public:
   RedBlackTree(void);
   ~RedBlackTree(void);

   void insert(int);
   void remove(int);

private:
   TreeNode* root;
   
   TreeNode* getGrandparent(TreeNode*) const;
   TreeNode* getUncle(TreeNode*) const;

   TreeNode* findInsertLocation(TreeNode*, int) const;

   TreeNode* makeNewNode(int) const;

   void rebalance(TreeNode*);
   void rebalanceCase3(TreeNode*);
   void rebalanceCase4(TreeNode*);
   void rebalanceCase5(TreeNode*);

   TreeNode* rotateLeft(TreeNode*);
   TreeNode* rotateRight(TreeNode*);

   rebalanceCase determineRebalanceCase(TreeNode*) const;

};

