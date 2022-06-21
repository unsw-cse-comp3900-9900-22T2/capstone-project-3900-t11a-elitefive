import React from 'react'

type Props = {
  width?: number;
  height?: number;
  stroke?: string;
  fill?: string;
  transform?: string;
  onClick?: () => void;
}

const DEFAULT_WIDTH = 110;
const DEFAULT_HEIGHT = 120;
const DEFAULT_STROKE = "#824ACB"
const DEFAULT_FILL = "#404348"
const DEFAULT_TRANSFORM = ""
const DEFAULT_STROKE_WIDTH = 3;

export default function HexCell(props: Props) {
  const { 
    width,
    height,
    stroke,
    fill,
    transform,
    onClick,
  } = props;

  // generates path for a perfect hexagon
  const getPathCoords = () => {
    const w = width ? width : DEFAULT_WIDTH;
    const h = height ? height: DEFAULT_HEIGHT;
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
    <svg 
      width={width? width: DEFAULT_WIDTH} 
      height={height? height: DEFAULT_HEIGHT}
      onClick={onClick}
    >
      <polygon
        points={getPathCoords()}
        stroke={stroke? stroke : DEFAULT_STROKE}
        fill={fill? fill : DEFAULT_FILL}
        transform={transform? transform: DEFAULT_TRANSFORM}
        strokeWidth={DEFAULT_STROKE_WIDTH}
      />
    </svg>
  )
}