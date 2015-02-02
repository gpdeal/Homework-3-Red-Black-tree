#include "RedBlackTree.h"


RedBlackTree::RedBlackTree(void) {
   root = nullptr;
}


RedBlackTree::~RedBlackTree(void) {
}

void RedBlackTree::insert(int value) {
   TreeNode* newNode = new TreeNode;
   newNode->data = value;
   
   if(root == nullptr) {
      root = newNode;
   } else {
      TreeNode* parentNode = findInsertLocation(root, value);
      if (value < parentNode->data) { parentNode->leftChild = newNode; }
      else { parentNode->rightChild = newNode; }
      newNode->parent = parentNode;
   }


}

TreeNode* RedBlackTree::getGrandparent(TreeNode* node) const {
   
   if (node == nullptr || node->parent == nullptr) { return nullptr; }

   return node->parent->parent;
}


TreeNode* RedBlackTree::getUncle(TreeNode* node) const {

   TreeNode* grandparent = getGrandparent(node);

   if (grandparent == nullptr) { return nullptr; }

   if (grandparent->leftChild == node->parent) {
      return grandparent->rightChild;
   } else {
      return grandparent->leftChild;
   }
}




TreeNode* RedBlackTree::findInsertLocation(TreeNode* node, int value) const {
   
   if(value < node->data) {
      if(node->leftChild == nullptr) { return node; }
      return findInsertLocation(node->leftChild, value);
   } else {
      if(node->rightChild == nullptr) { return node; }
      return findInsertLocation(node->rightChild, value);
   }

}

TreeNode* RedBlackTree::makeNewNode(int data) const {
   TreeNode* node = new TreeNode;
   node->data = data;
   return node;
}

void RedBlackTree::rebalance(TreeNode* node) {
   
   rebalanceCase rebCase = determineRebalanceCase(node);

   if (rebCase == case1) { root->color = black; }
   else if (rebCase == case3) {
      rebalanceCase3(node);
   } else if (rebCase == case4) {

   } else if (rebCase == case5) {

   }

}

void RedBlackTree::rebalanceCase3(TreeNode* node) {
   node->parent->color = red;
   getUncle(node)->color = red;
   getGrandparent(node)->color = black;
   rebalance(getGrandparent(node));
}


void RedBlackTree::rebalanceCase4(TreeNode* node) {
   TreeNode* grandparent = getGrandparent(node);
   
   if (node == node->parent->rightChild) {
      grandparent->leftChild = rotateLeft(node->parent);
      grandparent->leftChild->parent = grandparent;
      node = node->leftChild;
   } else {
      grandparent->rightChild = rotateRight(node->parent);
      grandparent->rightChild->parent = grandparent;
      node = node->rightChild;
   }

   rebalanceCase5(node);
}


void RedBlackTree::rebalanceCase5(TreeNode* node) {
   TreeNode* grandparent = getGrandparent(node);
   TreeNode* newNode;
   TreeNode* gGrandparent = grandparent->parent;
   bool isLeftChild;
   if (gGrandparent != nullptr) {
      isLeftChild = grandparent == gGrandparent->leftChild;
   }

   if (node == node->parent->rightChild) {
      newNode = rotateLeft(grandparent);
      newNode->leftChild->color = red;
   } else {
      newNode = rotateRight(grandparent);
      newNode->rightChild->color = red;
   }
   newNode->color = black;
   newNode->parent = gGrandparent;

   if (gGrandparent == nullptr) { root = newNode; } 
   else if (isLeftChild) { gGrandparent->leftChild = newNode; }
   else { gGrandparent->rightChild = newNode; }
}





rebalanceCase RedBlackTree::determineRebalanceCase(TreeNode* node) const {
   if (node == root) { return case1; }

   TreeNode* parent = node->parent;
   if (parent->color == black) { return case2; }
   
   TreeNode* uncle = getUncle(node);
   if (parent->color == red && uncle->color == red) { return case3; }

   TreeNode* grandparent = getGrandparent(node);
   if ((node == parent->rightChild && parent == grandparent->leftChild)
      || (node == parent->leftChild && parent == grandparent->rightChild)) {
      return case4; 
   }

   return case5;   
}

TreeNode* RedBlackTree::rotateLeft(TreeNode* node) {
   TreeNode* oldChild = node->rightChild;

   node->rightChild = oldChild->leftChild;
   if (node->rightChild != nullptr) { node->rightChild->parent = node; }

   oldChild->leftChild = node;
   node->parent = oldChild;

   return oldChild;
}


TreeNode* RedBlackTree::rotateRight(TreeNode* node) {
   TreeNode* oldChild = node->leftChild;

   node->leftChild = oldChild->rightChild;
   if (node->leftChild != nullptr) { node->leftChild->parent = node; }

   oldChild->rightChild = node;
   node->parent = oldChild;

   return oldChild;
}
