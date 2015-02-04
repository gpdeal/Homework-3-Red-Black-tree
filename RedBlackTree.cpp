#include "RedBlackTree.h"

TreeNode* RedBlackTree::nullLeaf = new TreeNode;

RedBlackTree::RedBlackTree(void) {
   root = nullptr;

   nullLeaf->color = black;
   nullLeaf->leftChild = nullptr;
   nullLeaf->rightChild = nullptr;
   nullLeaf->isNullLeaf = true;
}


RedBlackTree::~RedBlackTree(void) {
}

bool RedBlackTree::insert(int value) {
   TreeNode* newNode = makeNewNode(value);
   if (newNode == nullptr) { return false; }
   
   if(root == nullptr) {
      root = newNode;
   } else {
      TreeNode* parentNode = findInsertLocation(root, value);

      if (value < parentNode->data) { parentNode->leftChild = newNode; }
      else { parentNode->rightChild = newNode; }

      newNode->parent = parentNode;
   }

   insRebalance(newNode);
   return true;
}


bool RedBlackTree::remove(int value) {
   
   TreeNode* node = retrieve(value);
   if (node == nullptr) { return false; }


   if (!node->leftChild->isNullLeaf && !node->rightChild->isNullLeaf) {
      removeNodeWithTwoChildren(node);
   } else {
      remove(node);
   }

   return true;
}

void RedBlackTree::remove(TreeNode* node) {
   TreeNode* child = 
      node->leftChild->isNullLeaf ? node->rightChild : node->leftChild;

   replace(node, child);

   if (node->color == black) {
      if (child->color == red) {
         child->color = black;
      } else {
         remRebalance(child);
      }
   }

   delete node;
}


void RedBlackTree::removeNodeWithTwoChildren(TreeNode* node) {
   TreeNode* current = node->leftChild;

   while (!current->rightChild->isNullLeaf) {
      current = current->rightChild;
   }

   node->data = current->data;

   remove(current);
}


void RedBlackTree::replace(TreeNode* oldNode, TreeNode* newNode) {
   
   if (oldNode == root) { 
      root = newNode; 
      if (!newNode->isNullLeaf) { newNode->parent = nullptr; }
   }
   else {
      if (isRightChild(oldNode)) {
         oldNode->parent->rightChild = newNode;   
      } else {
         oldNode->parent->leftChild = newNode;
      }
      if (!newNode->isNullLeaf) { newNode->parent = oldNode->parent; }
   }
}



bool RedBlackTree::contains(int value) const {
   return retrieve(value) != nullptr;
}




TreeNode* RedBlackTree::getGrandparent(TreeNode* node) const {
   
   if (node == nullptr || node->parent == nullptr) { return nullptr; }

   return node->parent->parent;
}


TreeNode* RedBlackTree::getUncle(TreeNode* node) const {

   if (node == nullptr || (node->parent == nullptr || node->parent->parent == nullptr)) {
      return nullptr;
   }


   TreeNode* grandparent = getGrandparent(node);

   if (grandparent == nullptr) { return nullptr; }

   if (isRightChild(node->parent)) {
      return grandparent->leftChild;
   } else {
      return grandparent->rightChild;
   }
}


TreeNode* RedBlackTree::getSibling(TreeNode* node) const {
   if (node == nullptr || node->parent == nullptr) { return nullptr; }

   if (isRightChild(node)) { return node->parent->leftChild; }
   else { return node->parent->rightChild; }

}



TreeNode* RedBlackTree::retrieve(int value) const {
   return retrieve(root, value);
}

TreeNode* RedBlackTree::retrieve(TreeNode* node, int value) const {

   if (node->isNullLeaf) { return nullptr; }

   if (node->data == value) { return node; }

   if (value < node->data) { return retrieve(node->leftChild, value); }
   else { return retrieve(node->rightChild, value); }

}


TreeNode* RedBlackTree::findInsertLocation(TreeNode* node, int value) const {
   
   if(value < node->data) {
      if(node->leftChild->isNullLeaf) { return node; }
      return findInsertLocation(node->leftChild, value);
   } else {
      if(node->rightChild->isNullLeaf) { return node; }
      return findInsertLocation(node->rightChild, value);
   }

}

TreeNode* RedBlackTree::makeNewNode(int data) const {
   TreeNode* node = new TreeNode;
   
   if (node != nullptr) {
      node->data = data;
      node->isNullLeaf = false;
      node->leftChild = nullLeaf;
      node->rightChild = nullLeaf;
      node->parent = nullptr;
   }

   return node;
}

void RedBlackTree::insRebalance(TreeNode* node) {
   
   insRebalanceCase rebCase = determineInsRebalanceCase(node);

   if (rebCase == inscase1) { root->color = black; }
   else if (rebCase == inscase3) {
      insRebalanceCase3(node);
   } else if (rebCase == inscase4) {
      insRebalanceCase4(node);
   } else if (rebCase == inscase5) {
      insRebalanceCase5(node);
   }

}

void RedBlackTree::insRebalanceCase3(TreeNode* node) {
   node->parent->color = black;
   getUncle(node)->color = black;
   getGrandparent(node)->color = red;
   insRebalance(getGrandparent(node));
}


void RedBlackTree::insRebalanceCase4(TreeNode* node) {
   TreeNode* grandparent = getGrandparent(node);
   
   if (isRightChild(node)) {
      grandparent->leftChild = rotateLeft(node->parent);
      node->parent = grandparent;
      node = node->leftChild;
   } else {
      grandparent->rightChild = rotateRight(node->parent);
      node->parent = grandparent;
      node = node->rightChild;
   }

   insRebalanceCase5(node);
}


void RedBlackTree::insRebalanceCase5(TreeNode* node) {
   TreeNode* grandparent = getGrandparent(node);
   TreeNode* rotatedNode;
   TreeNode* gGrandparent = grandparent->parent;
   bool gpIsLeftChild;
   if (gGrandparent != nullptr) {
      gpIsLeftChild = !isRightChild(grandparent);
   }

   if (isRightChild(node)) {
      rotatedNode = rotateLeft(grandparent);
      rotatedNode->leftChild->color = red;
   } else {
      rotatedNode = rotateRight(grandparent);
      rotatedNode->rightChild->color = red;
   }
   rotatedNode->color = black;
   rotatedNode->parent = gGrandparent;

   if (gGrandparent == nullptr) { root = rotatedNode; } 
   else if (gpIsLeftChild) { gGrandparent->leftChild = rotatedNode; }
   else { gGrandparent->rightChild = rotatedNode; }
}


void RedBlackTree::remRebalance(TreeNode* node) {
   
   remRebalanceCase rebcase = determineRemRebalanceCase(node);

   if (rebcase == remcase2) {
      remRebalanceCase2(node);
   } else if (rebcase == remcase3) {
      remRebalanceCase3(node);
   } else if (rebcase == remcase4) {
      remRebalanceCase4(node);
   } else if (rebcase == remcase5) {
      remRebalanceCase5(node);
   } else {
      remRebalanceCase6(node);
   }

}


void RedBlackTree::remRebalanceCase2(TreeNode* node) {
   
   TreeNode* grandparent = getGrandparent(node);
   bool parentWasRightChild;
   TreeNode* rotatedNode;
   if (grandparent != nullptr) {
      parentWasRightChild = isRightChild(node->parent);
   }

   node->parent->color = red;
   getSibling(node)->color = black;

   if (isRightChild(node)) {
      rotatedNode = rotateRight(node->parent);
   } else {
      rotatedNode = rotateLeft(node->parent);
   }

   if (grandparent == nullptr) { root = rotatedNode; }

   else if (parentWasRightChild) {
      grandparent->rightChild = rotatedNode;
   } else {
      grandparent->leftChild = rotatedNode;
   }

   rotatedNode->parent = grandparent;

   remRebalance(node);
}


void RedBlackTree::remRebalanceCase3(TreeNode* node) {

   getSibling(node)->color = red;

   remRebalance(node->parent);
}

void RedBlackTree::remRebalanceCase4(TreeNode* node) {

   node->parent->color = black;

   getSibling(node)->color = red;
}

void RedBlackTree::remRebalanceCase5(TreeNode* node) {
   TreeNode* rotatedNode;

   if (isRightChild(node)) {

   }

}

void RedBlackTree::remRebalanceCase6(TreeNode* node) {

}



insRebalanceCase RedBlackTree::determineInsRebalanceCase(TreeNode* node) const {
   if (node == root) { return inscase1; }

   TreeNode* parent = node->parent;
   if (parent->color == black) { return inscase2; }
   
   TreeNode* uncle = getUncle(node);
   if (parent->color == red && uncle->color == red) { return inscase3; }

   TreeNode* grandparent = getGrandparent(node);
   if ((node == parent->rightChild && parent == grandparent->leftChild)
      || (node == parent->leftChild && parent == grandparent->rightChild)) {
      return inscase4; 
   }

   return inscase5;   
}


remRebalanceCase RedBlackTree::determineRemRebalanceCase(TreeNode* node) const {
   
   if (node == root) { return remcase1; }

   TreeNode* sibling = getSibling(node);
   if (sibling->color == red) { return remcase2; }

   if (node->parent->color == black && (sibling->leftChild->color == black 
      && sibling->rightChild->color == black)) {
         return remcase3;
   }

   if (node->parent->color == red && (sibling->leftChild->color == black 
      && sibling->rightChild->color == black)) {
         return remcase4;
   }

   if ((isRightChild(sibling) && sibling->leftChild->color == red)
      || (!isRightChild(sibling) && sibling->rightChild->color == red)) {
         return remcase5;
   }

   return remcase6;

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


bool RedBlackTree::isRightChild(TreeNode* node) const {
   return node->parent->rightChild == node;
}



void RedBlackTree::printTree() const {
   printTree(root);
}


void RedBlackTree::printTree(TreeNode* node) const {
   if(node != nullptr && !node->isNullLeaf) {
      printTree(node->leftChild);
      std::cout << node->data << std::endl;
      printTree(node->rightChild);
   }
}



void RedBlackTree::validateTree() const {
   validateTree(root, 0);
}


void RedBlackTree::validateTree(TreeNode* node, int blacks) const {
   
   if (node->isNullLeaf) {
      std::cout << blacks + 1 << std::endl;
   } else {
      if (node->color == black) { blacks++; }
      validateTree(node->leftChild, blacks);
      validateTree(node->rightChild, blacks);
   }
}
