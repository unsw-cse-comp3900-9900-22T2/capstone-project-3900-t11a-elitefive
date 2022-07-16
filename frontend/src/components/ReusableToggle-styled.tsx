import styled from "styled-components";

export type toggleProps = {
  background?: string;
  width?: number;
  height?: number;
  color?: string;
}


export const StyledToggle = styled.div<toggleProps>`
    width: ${props=>props.width?props.width:"120"}px;
    height: ${props=>props.height?props.height+"px":"auto"};
    background: ${props=>props.background?props.background:"var(--textbox-dark)"};
    color: ${props=>props.color?props.color:"white"};
    display: flex;
    justify-content: center;
    align-items: center;
    border-radius: 10px;
    &:hover {
    background: #EFEEF3;
    color: black;
    transform: scale(1.04);
    }
    &:active {
    background: #C8D1FA;
    color: #7482CB;
    transform: scale(0.96);
    }
    cursor: pointer;
`;