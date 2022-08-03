import * as React from 'react';
import ToggleButton from '@mui/material/ToggleButton';
import ToggleButtonGroup from '@mui/material/ToggleButtonGroup';
import ReusaleToggleButton from './ReusableToggleButton';

type Props = {
  children?: React.ReactElement | any;
}

export default function ReusableToggleButtonGroup({children}: Props) {
  const [alignment, setAlignment] = React.useState('web');

  const handleChange = (
    event: React.MouseEvent<HTMLElement>,
    newAlignment: string,
  ) => {
    setAlignment(newAlignment);
  };

  return (
    <ToggleButtonGroup
      value={alignment}
      exclusive
      onChange={handleChange}
    > {children}
    </ToggleButtonGroup>
  );
}