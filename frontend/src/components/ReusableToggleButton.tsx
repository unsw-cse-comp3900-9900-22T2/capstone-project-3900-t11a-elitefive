import * as React from 'react';
import ToggleButton from '@mui/material/ToggleButton';
import ToggleButtonGroup from '@mui/material/ToggleButtonGroup';
import styled from 'styled-components';
import {StyledToggle} from './ReusableToggle-styled';


type Props = {
  children?: React.ReactElement | any;
  onClick?: (...args: any) => void;
}

export default function ReusableToggleButton({children, onClick, ...styleProps}: Props) {
  return (
      <StyledToggle
        onClick={onClick}
      >
        {children}
      </StyledToggle>
  );
}