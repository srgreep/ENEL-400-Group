using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;




public class FinalScript : MonoBehaviour
{
    public GameObject knuckle, joint1, joint2;
    public UDPReceive udpReceive;
    float last_roatation = 0;
    public int index;
    void Start()
    {
       
 
    }

    void Update()
    {
  
        try
        {
            float incoming = -1*float.Parse(udpReceive.points[index]);
            if(index == 4)
            {
                
                float k = mapRange(30, incoming - last_roatation);
                float j1 = mapRange(55, incoming - last_roatation);
                float j2 = mapRange(80, incoming - last_roatation);
                

                joint1.transform.Rotate(0, -j1/2, -j1/2, Space.World);
                joint2.transform.Rotate(0, -j2/2, -j2/2, Space.World);
                last_roatation = incoming;
            }
            else
            {
                float k = mapRange(90, incoming-last_roatation);
                float j1 = mapRange(55, incoming-last_roatation);
                float j2 = mapRange(80, incoming-last_roatation);
                knuckle.transform.Rotate(k, 0, 0, Space.World);
                joint1.transform.Rotate(j1, 0, 0, Space.World);
                joint2.transform.Rotate(j2, 0, 0, Space.World);
                last_roatation = incoming;

            }
        }
        catch(Exception err) { }
        
    }

    private float mapRange(float max_out, float x)
    {
        if(x==0) {return 0;}
        return (x) * max_out / 100;
    }
   
}
