import { Typography } from '@mui/material';
import React from 'react'
import styled from 'styled-components';

type Props = {
  variant: "top" | "bottom"
}

const Container = styled.div<Props>`
  position: absolute;
  ${props => (props.variant == "top") && `
    top: 0;
  `};
  ${props => (props.variant == "bottom") && `
  bottom: 0;
  `}
  display: flex;
  justify-content: center;
  align-items: center;
  
  background: var(--accent-darker);
  width: 300px;
  height: 50px;
  border-radius: 10px;
`;
  
  
export default function ScrollWidget({ variant }: Props) {

  const renderText = () => {
    switch(variant) {
      case "top": {
        return (<Typography>Scroll Up</Typography>)
      }
      case "bottom": {
        return (<Typography>Scroll Down</Typography>)
      }
      default: {
        return (<></>)
      }
    }
  }

  return (
    <Container variant={variant}>
      {renderText()}
    </Container>
  )
}