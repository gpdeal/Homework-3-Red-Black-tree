#include "RedBlackTree.h"


RedBlackTree::RedBlackTree(void) {
   root = nullptr;
   size = 0;
}


RedBlackTree::~RedBlackTree(void) {
   while (size != 0) {
      remove(root->data);
   }
}

bool RedBlackTree::insert(int value) {
   TreeNode* newNode = makeNewNode(value);
   if (newNode == nullptr || newNode->rightChild == nullptr) { return false; }
   
   if(root == nullptr) {
      root = newNode;
   } else {
      TreeNode* parentNode = findInsertLocation(root, value);

      if (value < parentNode->data) { 
         delete parentNode->leftChild;
         parentNode->leftChild = newNode; 
      }
      else { 
         delete parentNode->rightChild;
         parentNode->rightChild = newNode; 
      }

      newNode->parent = parentNode;
   }

   insRebalance(newNode);
   size++;
   return true;
}


bool RedBlackTree::remove(int value) {
   
   TreeNode* node = retrieve(value);
   if (node == nullptr) { return false; }


   if (!node->leftChild->isNullLeaf && !node->rightChild->isNullLeaf) {
      removeNodeWithTwoChildren(node);
   } else {
      removeNodeWithFewerThanTwoChildren(node);
   }

   size--;
   if (root->isNullLeaf) {
      delete root;
      root = nullptr;
   }

   return true;
}

void RedBlackTree::removeNodeWithFewerThanTwoChildren(TreeNode* node) {
   TreeNode* child = 
      node->leftChild->isNullLeaf ? node->rightChild : node->leftChild;

   if (isRightChild(child)) {
      delete node->leftChild;
   } else {
      delete node->rightChild;
   }

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

   removeNodeWithFewerThanTwoChildren(current);
}


void RedBlackTree::replace(TreeNode* oldNode, TreeNode* newNode) {
   
   if (oldNode == root) { 
      root = newNode; 
   }
   else {
      if (isRightChild(oldNode)) {
         oldNode->parent->rightChild = newNode;   
      } else {
         oldNode->parent->leftChild = newNode;
      }
   }

   newNode->parent = oldNode->parent;
}



bool RedBlackTree::contains(int value) const {
   return retrieve(value) != nullptr;
}




TreeNode* RedBlackTree::getGrandparent(TreeNode* node) const {
   
   if (node == nullptr || node->parent == nullptr) { return nullptr; }

   return node->parent->parent;
}


TreeNode* RedBlackTree::getUncle(TreeNode* node) const {

   if (node == nullptr || (node->parent == nullptr || getGrandparent(node) == nullptr)) {
      return nullptr;
   }

   return getSibling(node->parent);
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

   if (node == nullptr || node->isNullLeaf) { return nullptr; }

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
      node->leftChild = makeNullNode(node);
      node->rightChild = makeNullNode(node);
      node->parent = nullptr;
   }

   return node;
}


TreeNode* RedBlackTree::makeNullNode(TreeNode* parent) const {
   TreeNode* node = new TreeNode;

   if (node != nullptr) {
      node->isNullLeaf = true;
      node->leftChild = nullptr;
      node->rightChild = nullptr;
      node->color = black;
      node->parent = parent;
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
      rotateLeft(node->parent);
      node = node->leftChild;
   } else {
      rotateRight(node->parent);
      node = node->rightChild;
   }

   insRebalanceCase5(node);
}


void RedBlackTree::insRebalanceCase5(TreeNode* node) {
   TreeNode* parent = node->parent;
   TreeNode* grandparent = getGrandparent(node);

   if (isRightChild(node)) {
      rotateLeft(grandparent);
   } else {
      rotateRight(grandparent);
   }

   grandparent->color = red;
   parent->color = black;

}


void RedBlackTree::remRebalance(TreeNode* node) {
   
   remRebalanceCase rebCase = determineRemRebalanceCase(node);

   if (rebCase == remcase2) {
      remRebalanceCase2(node);
   } else if (rebCase == remcase3) {
      remRebalanceCase3(node);
   } else if (rebCase == remcase4) {
      remRebalanceCase4(node);
   } else if (rebCase == remcase5) {
      remRebalanceCase5(node);
   } else if (rebCase == remcase6) {
      remRebalanceCase6(node);
   }

}


void RedBlackTree::remRebalanceCase2(TreeNode* node) {
  
   node->parent->color = red;
   getSibling(node)->color = black;

   if (isRightChild(node)) {
      rotateRight(node->parent);
   } else {
      rotateLeft(node->parent);
   }

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
   
   TreeNode* sibling = getSibling(node);

   if (isRightChild(sibling)) {
      rotateRight(sibling);
   } else {
      rotateLeft(sibling);
   }

   sibling->color = red;
   sibling->parent->color = black;

   remRebalance(node);
}

void RedBlackTree::remRebalanceCase6(TreeNode* node) {

   TreeNode* parent = node->parent;

   if (isRightChild(node)) {
      rotateRight(parent);
   } else {
      rotateLeft(parent);
   }

   nodeColor parentOldColor = parent->color;
   parent->color = parent->parent->color;
   parent->parent->color = parentOldColor;
   getSibling(parent)->color = black;
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

   // if either: 
   // sibling is right child with a left red child and right black child
   // OR
   // sibling is a left child with a right red child and a left black child
   if ((isRightChild(sibling) 
      && sibling->leftChild->color == red
      && sibling->rightChild->color == black)
      || (!isRightChild(sibling) 
      && sibling->rightChild->color == red
      && sibling->leftChild->color == black)) {
         return remcase5;
   }

   return remcase6;

}



void RedBlackTree::rotateLeft(TreeNode* node) {
   TreeNode* parent = node->parent;
   bool nodeWasRightChild;
   if (parent != nullptr) {
      nodeWasRightChild = isRightChild(node);
   }
   
   TreeNode* oldChild = node->rightChild;

   node->rightChild = oldChild->leftChild;
   if (node->rightChild != nullptr) { node->rightChild->parent = node; }

   oldChild->leftChild = node;
   node->parent = oldChild;

   if (parent != nullptr) {
      if (nodeWasRightChild) { parent->rightChild = oldChild; }
      else { parent->leftChild = oldChild; }
   } else {
      root = oldChild;
   }

   oldChild->parent = parent;
}


void RedBlackTree::rotateRight(TreeNode* node) {
   TreeNode* parent = node->parent;
   bool nodeWasRightChild;
   if (parent != nullptr) {
      nodeWasRightChild = isRightChild(node);
   }
   
   TreeNode* oldChild = node->leftChild;

   node->leftChild = oldChild->rightChild;
   if (node->leftChild != nullptr) { node->leftChild->parent = node; }

   oldChild->rightChild = node;
   node->parent = oldChild;

   if (parent != nullptr) {
      if (nodeWasRightChild) { parent->rightChild = oldChild; }
      else { parent->leftChild = oldChild; }
   } else {
      root = oldChild;
   }


   oldChild->parent = parent;
}


bool RedBlackTree::isRightChild(TreeNode* node) const {
   return node->parent->rightChild == node;
}



void RedBlackTree::printTree() const {
   if (size != 0) {
      std::queue<TreeNode*> q;
      int depth = 0;

      if (root != nullptr) { q.push(root); }

      while (!q.empty()) {
         TreeNode* current = q.front();
         q.pop();
         std::cout << current->data << ", " << current->color << std::endl;

         if (!current->leftChild->isNullLeaf) { q.push(current->leftChild); }
         if (!current->rightChild->isNullLeaf) { q.push(current->rightChild); }

      }
   }
}


void RedBlackTree::printTree(TreeNode* node, int depth) const {
   if(node != nullptr && !node->isNullLeaf) {
      printTree(node->leftChild, depth + 1);
      std::cout << node->data << ", " << depth << ", " << node->color << std::endl;
      printTree(node->rightChild, depth + 1);
   }
}



void RedBlackTree::validateTree() const {
   validateTree(root, 0);
}


void RedBlackTree::validateTree(TreeNode* node, int blacks) const {
   
   if (size != 0) {
      if (node->isNullLeaf) {
         std::cout << blacks + 1 << std::endl;
      } else {
         if (node->color == black) { blacks++; }
         validateTree(node->leftChild, blacks);
         validateTree(node->rightChild, blacks);
      }
   }
}
