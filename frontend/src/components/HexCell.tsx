import React from 'react'

type Props = {
  width?: number;
  height?: number;
  stroke?: string;
  fill?: string;
}

const DEFAULT_w = 110;
const DEFAULT_h = 120;
const DEFAULT_STROKE = "white"
const DEFAULT_FILL = "black"


export default function HexCell(props: Props) {
  const { 
    width,
    height,
    stroke,
    fill
  } = props;

  // generates path for a perfect hexagon
  const getPathCoords = () => {
    const w = width ? width : DEFAULT_w;
    const h = height ? height: DEFAULT_h
    const a = [w/2, 0];
    const b = [w, h/4];
    const c = [w, h * 3/4];
    const d = [w/2, h];
    const e = [0, h * 3/4];
    const f = [0, h / 4];
  
    const anchors = [a,b,c,d,e,f];
    let pathString = "";
    for(const tuple of anchors) {
      const tupleAsString = tuple[0]+","+tuple[1];
      pathString += tupleAsString;
      pathString += " ";
    }
    pathString.trim();
    return pathString;
  }

  return (
    <svg>
      <polygon
        points={getPathCoords()}
        stroke={stroke? stroke : DEFAULT_STROKE}
        fill={fill? fill : DEFAULT_FILL}
      />
    </svg>
  )
}