# Binary-search trees
class TreeNode(object):
	value:int = 0
	left:"TreeNode" = None
	right:"TreeNode" = None

	def insert(self:"TreeNode", x:int) -> bool:
		if x < self.value:
			if self.left is None:
				self.left = makeNode(x)
				return True
			else:
				return self.left.insert(x)
		elif x > self.value:
			if self.right is None:
				self.right = makeNode(x)
				return True
			else:
				return self.right.insert(x)
		return False

	def contains(self:"TreeNode", x:int) -> bool:
		if x < self.value:
			if self.left is None:
				return False
			else:
				return self.left.contains(x)
		elif x > self.value:
			if self.right is None:
				return False
			else:
				return self.right.contains(x)
		else:
			return True

class Tree(object):
	root:TreeNode = None
	size:int = 0

	def insert(self:"Tree", x:int) -> object:
		if self.root is None:
			self.root = makeNode(x)
			self.size = 1
		else:
			if self.root.insert(x):
				self.size = self.size + 1

	def contains(self:"Tree", x:int) -> bool:
		if self.root is None:
			return False
		else:
			return self.root.contains(x)

def makeNode(x: int) -> TreeNode:
	b:TreeNode = None
	b = TreeNode()
	b.value = x
	return b


# Input parameters
n:int = 100
c:int = 4

# Data
t:Tree = None
i:int = 0
k:int = 37813

# Crunch
t = Tree()
while i < n:
	t.insert(k)
	k = (k * 37813) % 37831
	if i % c != 0:
		t.insert(i)
	i = i + 1

print(t.size)

for i in [4, 8, 15, 16, 23, 42]:
	if t.contains(i):
		print(i)
