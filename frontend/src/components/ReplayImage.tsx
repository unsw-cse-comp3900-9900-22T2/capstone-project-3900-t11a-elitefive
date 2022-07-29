import { String } from 'lodash';
import React, { useEffect, useRef } from 'react'

type Props = {
  svg_data: string;
}

export default function ReplayImage({ svg_data }: Props) {

  const canvasRef = useRef<HTMLCanvasElement|null>(null);

  useEffect(() => {
    const canvas = canvasRef.current as HTMLCanvasElement;
    const ctx = canvas.getContext('2d');
    
    const a = 2 * Math.PI / 6;
    // const r = 50;
    const r = 18;
    const hex_width = r * Math.sqrt(3)/2;
    
    // We will pass you this data 
    const data_str = svg_data.split('')
    if(data_str.length == 0) return;
    const data: number[] = data_str.map((n) => parseInt(n));
    function init() {
      drawGrid(canvas.width, canvas.height);
    }
    init();
    
    function drawGrid(width: number, height:number) {
      let color = "#FF00FF";
      let cell = 0;
      let y = r + 10; // 10 offset from top of page
      for (let i = -4; i <= 4; i++) {
          let offset = Math.abs(i) * hex_width + r;
          for (let tiles = 0; tiles < 9 - Math.abs(i); tiles++) {
              if (data[cell] != 4) {    // 4 is a pothole. Simply don't render
                switch(data[cell]) {
                  case 0 : color = "#2b3135"; break; // Default fill
                  case 1 : color = "#f33880"; break; // Player 1
                  case 2 : color = "#1de254"; break; // Player 2
                  case 3 : color = "#00b9ca"; break; // Player 3
                }
                  drawHexagon(offset, y, color);
              }
              offset += hex_width * 2;
              cell += 1;
          }
          y += r * 1.5    // Shift row down
      }
    }
    
    function drawHexagon(x:number, y:number, fill:string) {
      if(!ctx) return;
      ctx.beginPath();
      for (let i = 0; i < 6; i++) {
        ctx.lineTo(x + r * Math.cos((a * i) + Math.PI/6), y + r * Math.sin((a * i) + Math.PI/6));
      }
      ctx.closePath();
      ctx.stroke();
      ctx.fillStyle = fill;
      ctx.fill();
      ctx.lineWidth = 3;
      ctx.strokeStyle = "#7a7a7a";
      ctx.stroke();
    }
  },[])

  return (
    <div style={{
      height: "100%",
      paddingLeft: "15px"
    }}>
      <canvas ref={canvasRef} height="265"/>
    </div>
  )
}