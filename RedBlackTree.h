#pragma once

#include <iostream>
#include "TreeNode.h"

enum insRebalanceCase { inscase1, inscase2, inscase3, inscase4, inscase5 };
enum remRebalanceCase { remcase1, remcase2, remcase3, remcase4, remcase5, remcase6 };

class RedBlackTree {
public:
   RedBlackTree(void);
   ~RedBlackTree(void);

   bool insert(int);
   bool remove(int);
   bool contains(int) const;

   void printTree() const;

   void validateTree() const;

private:
   TreeNode* root;
   static TreeNode* nullLeaf;
   
   TreeNode* getGrandparent(TreeNode*) const;
   TreeNode* getUncle(TreeNode*) const;
   TreeNode* getSibling(TreeNode*) const;

   TreeNode* findInsertLocation(TreeNode*, int) const;
   TreeNode* retrieve(int) const;
   TreeNode* retrieve(TreeNode*, int) const;

   TreeNode* makeNewNode(int) const;

   void insRebalance(TreeNode*);
   void insRebalanceCase3(TreeNode*);
   void insRebalanceCase4(TreeNode*);
   void insRebalanceCase5(TreeNode*);

   void remRebalance(TreeNode*);
   void remRebalanceCase2(TreeNode*);
   void remRebalanceCase3(TreeNode*);
   void remRebalanceCase4(TreeNode*);
   void remRebalanceCase5(TreeNode*);
   void remRebalanceCase6(TreeNode*);

   TreeNode* rotateLeft(TreeNode*);
   TreeNode* rotateRight(TreeNode*);

   bool isRightChild(TreeNode*) const;

   insRebalanceCase determineInsRebalanceCase(TreeNode*) const;
   remRebalanceCase determineRemRebalanceCase(TreeNode*) const;


   void printTree(TreeNode*) const;

   void validateTree(TreeNode*, int) const;

   void remove(TreeNode*);
   void removeNodeWithTwoChildren(TreeNode*);
   void replace(TreeNode*, TreeNode*);

};
