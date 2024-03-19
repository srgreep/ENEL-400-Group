using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;


public class HandController : MonoBehaviour
{
    public GameObject armature ;
    public List<Transform> armatureList = new List<Transform>();

    public GameObject hand ;
    public List<Transform> handList = new List<Transform>();


    public List<GameObject> points = new List<GameObject>();
    


    void Start()
    {
        //GetAllChilds(armature.transform,armatureList);
        //GetAllChilds(hand.transform,handList);

        //for (int i = 0 ; i < 3 ; i++)
        //{
        //    points.Add(armatureList[5+i].gameObject);
        //    points.Add(handList[5+i].gameObject);
        //    points.Add(handList[6+i].gameObject);
        //}

        //points.Add(armatureList[11].gameObject);
        //points.Add(handList[11].gameObject);
        //points.Add(handList[12].gameObject);

        //points.Add(armatureList[10].gameObject);
        //points.Add(handList[10].gameObject);
        //points.Add(handList[11].gameObject);

        //points.Add(armatureList[9].gameObject);
        //points.Add(handList[9].gameObject);
        //points.Add(handList[10].gameObject);

    }


    void Update()
    {
        //vector3 temp;

        //finger 1
        //for (int i = 0; i < 3; i++)
        //{
        //    temp = new vector3(getanglex(points[i * 3 + 1], points[i * 3 + 2]),
        //        points[i * 3].transform.localeulerangles.y, getanglez(points[i * 3 + 1], points[i * 3 + 2]));
        //    points[i * 3].transform.localeulerangles = temp;
        //}

        //finger 2

        //temp = new vector3(-getanglex(points[10], points[11]),
        //        points[9].transform.localeulerangles.y, -getanglez(points[10], points[11]));
        //points[9].transform.localeulerangles = temp;

        //temp = new vector3(-getanglex(points[13], points[14]),
        //        points[12].transform.localeulerangles.y, -getanglez(points[13], points[14]));
        //points[12].transform.localeulerangles = temp;

        //temp = new vector3(-getanglex(points[16], points[17]),
        //        points[15].transform.localeulerangles.y, -getanglez(points[16], points[17]));
        //points[15].transform.localeulerangles = temp;



    }

    //void GetAllChilds(Transform parent , List<Transform> Mylist)
    //{
    //    foreach (Transform child in parent)
    //    {
    //        Mylist.Add(child);
    //        GetAllChilds(child,Mylist);
    //    }
    //}
}
