#include <fstream>
#include <boost/timer.hpp>
#include <opencv2/imgcodecs.hpp>
#include <ros/ros.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>

#include <cv_bridge/cv_bridge.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <opencv2/core/core.hpp>
#include <image_transport/image_transport.h>

/*
    @Description:
    This code is used to grap stereo image toipc, and save them in the folder.
*/

struct ImgSave
{
    public:
        ImgSave(){std::cout<<"Build a new file!"<<std::endl;};
        void GrabStereo(const sensor_msgs::ImageConstPtr& msgLeft,const sensor_msgs::ImageConstPtr& msgRight);
        void StereoImageSave(const cv::Mat &ImgLeft, const cv::Mat &ImgRight, const double &timestamp);
    public:
        std::ofstream outfile_;  

        std::string save_file_path;
        std::string left_topic;
        std::string right_topic;
        int image_width;
        int image_height;
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ImageGrab");
    ros::start();
    ros::NodeHandle nh;

    ImgSave StereoImgSave;
    ImgSave *p = &StereoImgSave;
    ros::param::get("~save_file_path", p->save_file_path);
    ros::param::get("~left_topic", p->left_topic);
    ros::param::get("~right_topic", p->right_topic);
    ros::param::get("~image_width", p->image_width);
    ros::param::get("~image_height", p->image_height);
    // nh.param("image_width", w,640);
    // nh.param("image_height", h,360);

    p->outfile_.open(p->save_file_path + "timestamp.txt");
    
    std::cout<<"Here!!"<<std::endl;
    if (!p->outfile_) std::cout<<"Open file error!"<<std::endl;
    
    message_filters::Subscriber<sensor_msgs::Image> left_sub(nh, p->left_topic, 1);
    message_filters::Subscriber<sensor_msgs::Image> right_sub(nh, p->right_topic, 1);
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> sync_pol;
    message_filters::Synchronizer<sync_pol> sync(sync_pol(10), left_sub,right_sub);
    sync.registerCallback(boost::bind(&ImgSave::GrabStereo,p,_1,_2));
    
    ros::spin();
    ros::shutdown();
    p->outfile_.close();

    return 0;
}

void ImgSave::GrabStereo(const sensor_msgs::ImageConstPtr& msgLeft,const sensor_msgs::ImageConstPtr& msgRight)
{
    cv_bridge::CvImageConstPtr cv_ptrLeft;
    try
    {
        cv_ptrLeft = cv_bridge::toCvShare(msgLeft);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv_bridge::CvImageConstPtr cv_ptrRight;
    try
    {
        cv_ptrRight = cv_bridge::toCvShare(msgRight);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
   StereoImageSave(cv_ptrLeft->image,cv_ptrRight->image,cv_ptrLeft->header.stamp.toSec());
}

void ImgSave::StereoImageSave(const cv::Mat &ImgLeft, const cv::Mat &ImgRight, const double &timestamp)
{
    cv::Mat leftImg = ImgLeft;
    cv::Mat rightImg = ImgRight;
    double SaveTimeStamp;
    if ( leftImg.data==nullptr || rightImg.data==nullptr)
    std::cout<<"**********Image Errors!**********"<<std::endl;
    
    cv::cvtColor(leftImg,leftImg,CV_RGBA2GRAY);
    cv::cvtColor(rightImg,rightImg,CV_RGBA2GRAY);//zed的图片是4通道RGBA

    cv::resize(leftImg,leftImg,cv::Size(ImgSave::image_width,ImgSave::image_height),0,0);
    cv::resize(rightImg,rightImg,cv::Size(ImgSave::image_width,ImgSave::image_height),0,0);

    std::cout<<"ImageSize: "<<rightImg.size()<<std::endl;
     
    SaveTimeStamp = timestamp;
    std::cout<<"savetime: "<<SaveTimeStamp<<std::endl;
    std::string ImgName  = std::to_string(SaveTimeStamp);
    
    outfile_<<ImgName<<"\n";
    cv::imwrite(ImgSave::save_file_path+"image_0/"+ImgName+".png",leftImg);
    cv::imwrite(ImgSave::save_file_path+"image_1/"+ImgName+".png",rightImg);
}

