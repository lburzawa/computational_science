import numpy as np
import tensorflow as tf
import time
import sys

x = tf.placeholder(tf.float32, [None, 10000])
y_ = tf.placeholder(tf.float32, [None, 5])

x_image = tf.reshape(x, [-1,100,100,1])

W_conv1 = tf.Variable(tf.truncated_normal([5, 5, 1, 32], stddev=0.1))
b_conv1 = tf.Variable(tf.constant(0.1,shape=[32]))
h_conv1 = tf.nn.relu(tf.nn.conv2d(x_image, W_conv1,strides=[1, 1, 1, 1], padding='SAME') + b_conv1)
h_pool1 = tf.nn.max_pool(h_conv1, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')

W_conv2 = tf.Variable(tf.truncated_normal([5, 5, 32, 64], stddev=0.1))
b_conv2 = tf.Variable(tf.constant(0.1,shape=[64]))
h_conv2 = tf.nn.relu(tf.nn.conv2d(h_pool1, W_conv2,strides=[1, 1, 1, 1], padding='SAME') + b_conv2)
h_pool2 = tf.nn.max_pool(h_conv2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')

W_fc1 = tf.Variable(tf.truncated_normal([25 * 25 * 64, 1024], stddev=0.1))
b_fc1 = tf.Variable(tf.constant(0.1,shape=[1024]))
h_pool2_flat = tf.reshape(h_pool2, [-1, 25*25*64])
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

W_fc2 = tf.Variable(tf.truncated_normal([1024, 5], stddev=0.1))
b_fc2 = tf.Variable(tf.constant(0.1,shape=[5]))
keep_prob = tf.placeholder(tf.float32)
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)
y_conv = tf.matmul(h_fc1_drop, W_fc2) + b_fc2

cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels=y_, logits=y_conv))
train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
correct_prediction = tf.equal(tf.argmax(y_conv,1), tf.argmax(y_,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

sess = tf.InteractiveSession()
sess.run(tf.global_variables_initializer())

def prep_batch(file):
    batch_temp=[]
    for i in range(100):
        a=file.readline(); a=a[:-1]
        a=a.split(',')
        batch_temp.append(a)
    batch=np.array(batch_temp,dtype=np.float32)
    batch0=batch[:,:-5]
    batch1=batch[:,-5:]
    result=(batch0,batch1)
    return result

for i in range(10):
    train_accuracy=0.0
    file=open('train.csv','r')
    start_time=time.time()
    for j in range(4000):
        if j%400==0:
            print('This is batch %d and time elapsed is %f' % (j,time.time()-start_time))
            sys.stdout.flush()
            start_time=time.time()
        batch=prep_batch(file)
        train_step.run(feed_dict={x: batch[0], y_: batch[1], keep_prob: 0.5})
        train_accuracy += accuracy.eval(feed_dict={x: batch[0], y_: batch[1], keep_prob: 1.0})
    train_accuracy /= 4000
    print("epoch %d, training accuracy %g"%(i+1, train_accuracy))
    file.close()
  
    test_accuracy=0.0
    file=open('test.csv','r')
    for j in range(1000):
        batch=prep_batch(file)
        test_accuracy += accuracy.eval(feed_dict={x: batch[0], y_: batch[1], keep_prob: 1.0})
    test_accuracy /= 1000
    print("epoch %d, testing accuracy %g"%(i+1, test_accuracy))
    file.close()
    sys.stdout.flush()


