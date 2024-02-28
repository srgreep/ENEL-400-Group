using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Manager : MonoBehaviour
{
    public float pointer, middle, index, pinky;
    public string data = "[100,100,100,100]";
    public UDPReceive udpReceive;

    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            Application.Quit();
            Debug.Log("Goodbye");
        }

        string[] points = udpReceive.data.Split(','); 
    }
}
